/*
 * THIS ARE FILE EXTRACTION IDEATION
 * THIS ARE DESIGN ARE NOT AS HOW PROFESSIONAL USED
 * THE AIM OF THIS ARE JUST FOR STUDY PURPOSE ONLY
 *
 * THIS ARE NOT COMPLETED YET, BUT THE SEQUENCE ARE:
 *    1. EXTRACT BYTE DATA (THIS ONE)
 *    2. DECODE BYTE DATA  (UPCOMING)
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct ImageLoader{
   FILE** file;
   long size;
   char* buffer;
} ImageLoader;

struct ImageChecker{
   unsigned char* check;
   size_t byte;
} ImageChecker;

const unsigned char png[8] = {137, 80, 78, 71, 13, 10, 26, 10}; 
const unsigned char jpg[3] = {255, 216, 255};
const unsigned char bmp[2] = {66, 77};

//opening and checking file size
void fileInitialize( struct ImageLoader* img, struct ImageChecker* img_checker,
                     int n, char* path_file1, char* path_file2){
   
   /*
    * n represent how much FILE* we wanted to have in our FILE* pointer.
    * file need to allocated using malloc because we use pointer FILE*.
    * But its best practice to use calloc
    *    Example: img->file = calloc(n, sizeof(FILE*));
    *    Since variable are FILE** so inside of it must have FILE*,
    *    hence need FILE* byte size. 
    */

   img->file = malloc(n*sizeof(FILE*));

   /*
    * Optional (Creating space to to check opened images)
    */

   img_checker->check = malloc(8);

   /*
    * Opening file, standard operation.
    * Needed @path to desired file, behaviour that we want it to do.
    * In this case are:
    *    1. rb = Read Binary (Read-Only)
    *    2. w = Write
    */

   img->file[0] = fopen(path_file1,"rb");
   img->file[1] = fopen(path_file2, "w");

   /*
    * Opening file, standard operation.
    * Needed @path to desired file, behaviour that we want it to do.
    * In this case are:
    *    1. rb = Read Binary (Read-Only)
    *    2. w = Write
    *
    * After file has been open, we need to read whatever data inside those file.
    * In this case:
    *    1. using fseek()
    *       a) read of file[0]
    *       b) starting from position 0
    *       c) then check until the end of file
    *    
    *    2. using ftell()
    *    once we check the file, we need to have specific size to be used
    *    for our buffer, using malloc or calloc accordingly.
    *
    *    3. using rewind()
    *    rewind are just stating that the position should be at the very first
    *    of line inside the file.
    */

   fseek(img->file[0], 0, SEEK_END);
   img->size = ftell(img->file[0]);
   rewind(img->file[0]);

   /*
    * Reading and transfering 8 byte on first line of file images into check stream
    */

   img_checker->byte = fread(img_checker->check, 1, 8, img->file[0]); 
}

//free dynamic allocated data
void fileRelease(struct ImageLoader* img, struct ImageChecker* img_checker){
   
   /*
    * Every FILE* that been used/open need to be close once we finished.
    */

   fclose(img->file[0]);
   fclose(img->file[1]);

   /*
    * All data that been dynamically allocated needed to be free once we finished.
    * If not we might get unexpectedly behaviour from the apps.
    *    Example: Such as crash, segmented augmentation, etc...
   */

   free(img_checker->check);
   free(img->buffer);
   free(img->file);
}

//byte extraction from image using pointer
void fileOperating(  struct ImageLoader* img, struct ImageChecker* img_checker,
                     const unsigned char* f1, 
                     const unsigned char* f2,
                     const unsigned char* f3  ){

   /*
    * Buffer need to allocated using malloc.
    * But its best practice to use calloc
    *    Example: img->buffer = calloc(img->size + 1, sizeof(char));
    *    Since buffer are char* so it need char byte size which are 1.
   */

   img->buffer = malloc(img->size + 1);

   /*
    * Eventhough fgets() will read and extract all of the data inside a file,
    * But in this case, we want to see the byte design of the particular data.
    * We can just use fget() and move on. Using while loop are optional.
    * 
    * But the original idea are:
    *    1. Print those data in terminal
    *    2. Print those data into .txt file
    *
    * Since printf() would just stop printing once it hit the end of LINE,
    * Same goes if we want to print it into another FILE, where it just print first line.
    *    P.S: Meaning we lose data that needed to do what we want.
    *    
    * This particular reason to use while loop as it will keep printing until the end of FILE.  
   */

   while(fgets(img->buffer, sizeof(img->buffer), img->file[0])){
      //printf("%s", img->buffer);
      fputs(img->buffer, img->file[1]);
   }
   
   /*
    * Checking the file type
    */

   if(memcmp(img_checker->check, f1, 8) == 0){
      printf("This are PNG format\n");
   }
   else if(memcmp(img_checker->check, f2, 3) == 0){
      printf("This are JPG format\n");
   }
   else if(memcmp(img_checker->check, f3, 2) == 0){
      printf("This are BMP format\n");
   }

}

int main(void){

   struct ImageLoader image;
   struct ImageChecker image_checker;

   fileInitialize(&image, &image_checker,
                  2,
                  "src/image.png",
                  "src/image.txt");

   fileOperating(&image, &image_checker, png, jpg, bmp);
   fileRelease(&image, &image_checker);

/* 
 * Original Idea Before Refactoring
 *
 * image.num = 2;
 * image.file = malloc(image.num*sizeof(FILE*));
 *
 * image.file[0] = fopen("image.png","rb");
 * image.file[1] = fopen("image.txt", "w");
 * fseek(image.file[0], 0, SEEK_END);
 * image.size = ftell(image.file[0]);
 * rewind(image.file[0]);
 *
 * image.buffer = malloc(image.size + 1);
 * while(fgets(image.buffer, sizeof(image.buffer), image.file[0])){
 *    printf("%s", image.buffer);
 *    fputs(image.buffer, image.file[1]);
 * }
 *
 * fclose(image.file[0]);
 * fclose(image.file[1]);
 * free(image.buffer);
 * free(image.file);
 *
*/
   return 0;
}
