#include "fs.h"
#include <fstream>
#include <string>
#include<iostream>

using namespace std;
fstream disk;

myFileSystem::myFileSystem(char diskName[16]) {
  // open the file with the above name
  // this file will act as the "disk" for your file system
  
  disk.open(diskName);
  // disk.seekg(0,ios::beg);

  if (disk.is_open() ){
  printf("IT IS OPEN");
  }
  else{
    // printf("IT IS CLOSED");
  }
}


int myFileSystem::create_file(char name[8], int size) {
  // create a file with this name and this size

  // high level pseudo code for creating a new file

  // Step 1: Check to see if we have sufficient free space on disk by
  //   reading in the free block list. To do this:
  // Move the file pointer to the start of the disk file.
  // Read the first 128 bytes (the free/in-use block information)
  // Scan the list to make sure you have sufficient free blocks to
  //   allocate a new file of this size


disk.seekg(0, ios_base::beg);
//   // read in the first 128 bytes
int count=0;
char  * blockbuffer= new char [128];
disk.read(blockbuffer,128);

  //iterate to find if there are sufficient blocks
for (int i=0;i<128;i++){
      // cout<<blockbuffer[i]<<"\n";
  // printf((char *) blockbuffer[i]);
  if (blockbuffer[i]==0){
    count+=1;
  }
}
printf("THE SIZE IS %d \n", count);

if (count < size){
  printf("COULD NOT B FOUND");
  return -1;
}
  // Step 2: we look for a free inode on disk
  // Read in an inode
  // Check the "used" field to see if it is free
  // If not, repeat the above two steps until you find a free inode
  // Set the "used" field to 1
  // Copy the filename to the "name" field
  // Copy the file size (in units of blocks) to the "size" field
disk.seekg(128,ios_base::beg);
idxNode * cur = new idxNode();
int j=0;
// idxNode * cur2= new idxNode()
// char * buf2= new char[sizeof(idxNode)];

for(j=0;j<16;j++){
// disk.read((char *)cur, sizeof(idxNode));
disk.read((char*) cur, sizeof(idxNode));
// cur= (idxNode *) buf2;
  
  if( cur->used == 0){
    break;
  }
}
// printf("IT STOPS AT %d", j);

if (cur->used!=0){
  return -1;
}

// //set the cur
cur->used= 1;
for( int i=0;i<8;i++){
  cur->name[i]=name[i];
}
cur->size= size;


  // Step 3: Allocate data blocks to the file
  // for(i=0;i<size;i++)
  // Scan the block list that you read in Step 1 for a free block
  // Once you find a free block, mark it as in-use (Set it to 1)
  // Set the blockPointer[i] field in the inode to this block number.
  // end for
int sizeyet=0;
for (int i=0;i<128;i++){
  if(sizeyet == size){
    break;
  }
  if (blockbuffer[i]==0){
    blockbuffer[i]=1;
    printf("we assigned block number %d,", i);
    cur->blockPointers[sizeyet] = i;
    sizeyet+=1;
  }
}

// string s= "";
// for (int i=0;i<1024;i++){
//             s=s+ blockbuffer[i]; 
//           }
//     cout<<s<<"\n";



//   // Step 4: Write out the inode and free block list to disk
//   // Move the file pointer to the start of the file
//   // Write out the 128 byte free block list
//   // Move the file pointer to the position on disk where this inode was stored
//   // Write out the inode

disk.seekp(0,ios_base::beg);
disk.write(blockbuffer,128);
int offset= 128 + sizeof(idxNode)*j ;
disk.seekp(offset,ios_base::beg);
disk.write((char *) cur, sizeof(idxNode)); 
return 1;

}  // End Create

int myFileSystem::delete_file(char name[8]) {
  // Delete the file with this name
  // Step 1: Locate the inode for this file
  // Move the file pointer to the 1st inode (129th byte)
  // Read in an inode
  // If the inode is free, repeat above step.
  // If the inode is in use, check if the "name" field in the
  //   inode matches the file we want to delete. If not, read the next
  //   inode and repeat

disk.seekg(128,ios_base::beg);
idxNode * cur= new idxNode();
int j=0;
// char * buf2= new char[sizeof(idxNode)];
int indicator=0;
for(j=0;j<16;j++){
disk.read((char*)cur, sizeof(idxNode));
// cur= (idxNode *) buf2;
if(cur->used==0){
  continue;
}
else{
  if (strcmp(name, cur->name)==0 ){
    indicator=1;
    break;
  }
}
}
if(indicator==0){
  return -1;
}
  // Step 2: free blocks of the file being deleted
  // Read in the 128 byte free block list (move file pointer to start
  //   of the disk and read in 128 bytes)
  // Free each block listed in the blockPointer fields as follows:
  // for(i=0;i< inode.size; i++)
  // freeblockList[ inode.blockPointer[i] ] = 0;

disk.seekg(0,ios::beg);
char  * blockbuffer= new char [128];
disk.read(blockbuffer,128);

  
for(int i=0;i<8;i++){
  int a = cur->blockPointers[i];
  if(a!=0){
    blockbuffer[a]=0;
  } 
}
  // Step 3: mark inode as free
  // Set the "used" field to 0.
cur->used=0;

  // Step 4: Write out the inode and free block list to disk
  // Move the file pointer to the start of the file
  // Write out the 128 byte free block list
  // Move the file pointer to the position on disk where this inode was stored
  // Write out the inode

disk.seekp(0,ios_base::beg);
disk.write(blockbuffer,128);
int offset= 128 + sizeof(idxNode)*j ;
disk.seekp(offset,ios_base::beg);
disk.write((char *) cur, sizeof(idxNode)); 
return 1;

}  // End Delete



int myFileSystem::ls() {

disk.seekg(128,ios_base::beg);
idxNode * cur = new idxNode();
int j=0;
// char * buf2= new char[sizeof(idxNode)];
for(j=0;j<16;j++){
disk.read((char*)cur, sizeof(idxNode));
// cur= (idxNode *) buf2;
if(cur->used ==1){
  printf(" %s \n",cur->name);
  printf("size is: %d \n",cur->size);
}
}
  // List names of all files on disk

  // Step 1: read in each inode and print
  // Move file pointer to the position of the 1st inode (129th byte)
  // for(i=0;i<16;i++)
  // Read in an inode
  // If the inode is in-use
  // print the "name" and "size" fields from the inode
  // end for
  return 1;
}  // End ls

int myFileSystem::read(char name[8], int blockNum, char buf[1024]) {
  // read this block from this file

  // Step 1: locate the inode for this file
  // Move file pointer to the position of the 1st inode (129th byte)
  // Read in an inode
  // If the inode is in use, compare the "name" field with the above file
  // If the file names don't match, repeat

disk.seekg(128,ios_base::beg);
idxNode * cur= new idxNode();
// char * buf2= new char[sizeof(idxNode)];
int indicator=0;
for(int j=0;j<16;j++){
disk.read((char*)cur, sizeof(idxNode));
// cur= (idxNode *) buf2;
if(cur->used==1){
  if  (strcmp(name, cur->name)==0){
    printf("The file was found indeed");
    indicator=1;
    break;
  }
}
}
if(indicator==0){
  return -1;
}
  // Step 2: Read in the specified block
  // Check that blockNum < inode.size, else flag an error
  // Get the disk address of the specified block
  // That is, addr = inode.blockPointer[blockNum]
  // Move the file pointer to the block location (i.e., to byte #
  //   addr*1024 in the file)
if(cur->size < blockNum){
  return -1;
}
int addr= cur->blockPointers[blockNum];
int offset= (addr*1024);
disk.seekg(offset,ios::beg);
disk.read(buf,1024);

  // Read in the block => Read in 1024 bytes from this location
  //   into the buffer "buf"
  return 1;
}  // End read





int myFileSystem::write(char name[8], int blockNum, char buf[1024]) {
  // write this block to this file

  // Step 1: locate the inode for this file
  // Move file pointer to the position of the 1st inode (129th byte)
  // Read in a inode
  // If the inode is in use, compare the "name" field with the above file
  // If the file names don't match, repeat

disk.seekg(128,ios_base::beg);
idxNode * cur= new idxNode();
int j=0;
// char * buf2= new char[sizeof(idxNode)];
int indicator=0;
// char * buf2= new char[sizeof(idxNode)];

for(int j=0;j<16;j++){
disk.read((char*)cur, sizeof(idxNode));
// cur= (idxNode *) buf2;
if(cur->used==1){
  if  (strcmp(name, cur->name)==0){
    printf("the file was found indeeed");
    indicator=1;
    break;
  }
}
}



if(indicator==0){
  return -1;
}

if(cur->size <= blockNum){
  return -1;
}
int addr= cur->blockPointers[blockNum];

int offset= addr*1024 ;

disk.seekp(offset,ios::beg);
disk.write(buf,1024);

  return 1;

  // Step 2: Write to the specified block
  // Check that blockNum < inode.size, else flag an error
  // Get the disk address of the specified block
  // That is, addr = inode.blockPointer[blockNum]
  // Move the file pointer to the block location (i.e., byte # addr*1024)

  // Write the block! => Write 1024 bytes from the buffer "buff" to
  //   this location
  // return 0;
}  // end write

int myFileSystem::close_disk() {
  // close the disk!
  disk.close();
  return 1;
  
}






  // int countname= 0;
  // for(int i= 0; i<8;i++){
  //   if(name[i]==cur->name[i]){
  //   countname+=1;
  // }
  // }
  // if(countname==8){
  //   indicator=1;
  //   break;
  // }
  // else{
  //   continue;
  // }



// for(j=0;j<16;j++){
// disk.read(buf2, sizeof(idxNode));
// cur= (idxNode *) buf2;
// if(cur->used==0){
//   continue;
// }
// else{
//   int countname= 0;
//   for(int i= 0; i<8;i++){
//     if(name[i]==cur->name[i]){
//     countname+=1;
//   }
//   }
//   if(countname==8){
//     indicator=1;
//     break;
//   }
//   // else{
//   //   continue;
//   // }
// }