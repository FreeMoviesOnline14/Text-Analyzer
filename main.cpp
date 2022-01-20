/*

Filename: main.cpp
Date created: 02/18/2021

Author(s): Marcellus Von Sacramento

Purpose: The purpose of this program is to read text from a file,
         search for unique word(s), keep track of the number of the unique word(s),
         sort the word(s) list, find the number of 1-3 character word(s),
         find the longest word(s), and finally display the output and write
         the output on an output file. This program has the capability to filter
         non-Alphabetic characters. And, will always convert the first letter
         of the words to uppercase.

         e.g.
              he3ll0o  ----> filter function ----> Hello
              w0or1ld  ----> filter function ----> World

Note: Empty files are allowed and will be checked.
      Even file with pure numeric(s) or pure punctuation(s)
      are allowed.

Program's known limitation:
- length of each line are fixed.
- length of each filenames are fixed.

- text separated by tab (\t) are considered as one whole token
  this is because the \t was not included in the strtok() delimiters.

   e.g.
      hello \t world ----> will result to ----> Helloworld
      bye \t world ----> will result to ----> Byeworld


Copyright(c)

Last date modified: 01/19/2022

*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <iomanip> // setw
#include <random>
#include <time.h> // seed
#include <conio.h> // getch()

using namespace std;

// global
const int line_Length = 501; // length per line
const int name_Size = 51;

// for main array template
struct Unique_Word
{
   int string_Length = 0;
   int found_Count = 0;
   char *the_Word = nullptr; // point to dynamically allocated char array
                             // longest known word in the world is 45 characters length.
                             // source: https://www.grammarly.com/blog/14-of-the-longest-words-in-english/
}; // end of struct


// tokenize passed CString
void tokenize_Line(char [], Unique_Word *&, int &, int &);

// remove non-Alphabetic characters
void filter_Characters(char []);

// validate existence of the word in the array
bool checkif_newUniq(const char [], const Unique_Word [], const int &, int &);

// expand array as new unique word is found
void expand_Arr(Unique_Word *&, int &);

// find longest word loc, 1-3 char words loc, word occured most loc. try const int &
void word_Info(const Unique_Word [], const int &, int &, int &, int &);

// display to console and write to file
void display_Output(const Unique_Word [], const int &, const int &, const int &, const int &); // try const int&
void write_Fout(ofstream &, const Unique_Word [], const int &, const int &, const int &, const int &); // try const int&


/***** sorting algorithms *****/
// sort the list using bubble sort algorithm
void BubbleSort(Unique_Word [], const int &);

// sort the list using QuickSort algorithm
void QuickSort(Unique_Word [], int, int);
int partition(Unique_Word [], int, int);

// for sorting algorithms above
void swap_Element(Unique_Word &, Unique_Word &);


int main()
{
   ifstream fin;
   ofstream fout;

   bool file_Empty; // assuming file is empty
   bool only_WS; // holds bool value if line contains only whitespace(s)

   char fin_Filename[name_Size];
   char fout_Filename[name_Size];
   char exit_String[] = "-1";

   char current_Line[line_Length]; // line to be passed for tokenizing

   // dynamic array pointer and initial size
   // dyna_Size can be used as total number of unique word
   Unique_Word *word_Array = nullptr;
   int dyna_Size;

   int total_Words; // hold total count of words found in the file
   int short_Count; // total count of 1-3 characters words.
   int long_Loc; // hold longest word index location.
   int most_Loc; // index location of the word that occurred most.

   // program start and will keep running until terminated by user
   do
   {
      // reset value of the following:
      dyna_Size   = 0;
      total_Words = 0;
      short_Count = 0;
      long_Loc    = 0;
      most_Loc    = 0;
      file_Empty  = true;
      only_WS     = true;

      // change this depending on OS
      // may be a bad thing to add this
      system("cls");

      // program termination instruction
      cout << "Enter \"-1\" on either of the following prompt to EXIT program" << "\n";

      do
      {
         cout << "\nEnter input file name: ";
         cin.getline(fin_Filename, name_Size);

         if(strcmp(fin_Filename, exit_String) == 0)
         {
            exit(EXIT_SUCCESS); // exit if user choose to end program
         }

         fin.open(fin_Filename);

         if(fin.is_open())
         {
            cout << "Opened file successfully!" << "\n";
         }
         else
         {
            cout << "Failed to open file!" << "\n";
         }
      }while(!fin.is_open());

      // this pretty much will always be true
      do
      {
         cout << "\nEnter output file name: ";
         cin.getline(fout_Filename, name_Size);

         //give user another chance to terminate program
         if(strcmp(fout_Filename, exit_String) == 0)
         {
            fin.close();
            exit(EXIT_SUCCESS);
         }

         fout.open(fout_Filename);

         if(fout.is_open())
         {
            cout << "Opened file successfully!" << "\n";
         }
         else
         {
            cout << "Failed to open file!" << "\n";
         }
      }while(!fout.is_open());

      cout << "\n";

      // read file and at the same time validate each line.
      // skip any non-complying lines
      while(fin.getline(current_Line, line_Length)) // implicit .eof() call
      {
         // if line is empty skip current line
         if(strlen(current_Line) == 0)
         {
            continue;
         }
         else // check if it's just a bunch of whitespace(s)
         {
            for(int i = 0; i < (strlen(current_Line)); i++)
            {
               if(!isspace(current_Line[i])) // if non-whitespace character found, break the loop. File is not empty.
               {
                  file_Empty = false;
                  only_WS = false;
                  break;
               }
            }
            if(only_WS) // bool value of only_WS reset to true each iteration
            {
               continue; // if line contains only bunch of whitespace(s), skip currentline
            }
         }

         // will not execute unless non-whitespace character(s) is/are found!
         cout << current_Line << "\n";
         fout << current_Line << "\n";
         tokenize_Line(current_Line, word_Array, total_Words, dyna_Size);
      }

      if(file_Empty) // bool value of file_Empty reset to true each iteration
      {
         cout << "\nFile is empty!" << "\n";
      }
      else if(!file_Empty && total_Words == 0) //  file may contain pure numerics or pure punctuations
      {
         cout << "\nFile is not empty. But, no words can be constructed!" << "\n";
         fout << "\nFile is not empty. But, no words can be constructed!" << "\n";
      }
      else // if alphabetic is found, call rest of functions
      {

         QuickSort(word_Array, 0, dyna_Size - 1);
         //BubbleSort(word_Array, dyna_Size); // commented out for testing
         word_Info(word_Array, dyna_Size, long_Loc, most_Loc, short_Count);
         display_Output(word_Array, dyna_Size, long_Loc, most_Loc, short_Count);
         write_Fout(fout, word_Array, dyna_Size, long_Loc, most_Loc, short_Count);

         delete [] word_Array; // deallocate memory

         word_Array = nullptr; // may be unnecessary since end of program
      }

      fin.close();
      fout.close();

      cout << "\n\nPress any key to read another file...";
      getch();

   }while(true);

} // end of main()

// process each line from input file.
void tokenize_Line(char current_Line[], Unique_Word *&word_Array, int &total_Words, int &dyna_Size)
{
   char *copy_currentLine = nullptr; // copy current_Line for strtok() to destroy
   char *current_Word = nullptr; // hold each token
   char *refined_Word = nullptr; // will hold the current_Word after
                                 // filtering out the non-Alphabetic
   int array_Index = 0;
   bool is_newUnique = true;

   // this may be unnecessary since we have no use for the current_Line
   copy_currentLine = new char[strlen(current_Line) + 1](); // create array and initialize

   // create copy of current line
   strcpy(copy_currentLine, current_Line);

   // retrieve first token
   current_Word = strtok(copy_currentLine, " .,;!?");

   // retrieve rest of the token until null is return
   while(current_Word != nullptr)
   {
      // filter out numeric and remove any case.
      filter_Characters(current_Word);

      /*
       Note:
         line containing pure numerics or pure punctuation will cause
         current_Word to have strlen(current_Word) == 0 after the filter_Characters() call
         Thus, no need to allocate memory that will obviously hold nothing
      */
      if(strlen(current_Word) == 0)
      {
         // take next token
         // go to next iteration, if there's any
         current_Word = strtok(nullptr, " .,;!?");
         continue;
      }
      else // something was formed during filtration
      {
         total_Words++; // update word count
      }

      /*
       Note:
         allocate memory for the refined word
         this ensure that we point at different blocks of char arrays,
         and not just to the single block of copy_currentLine array which
         when accidentally deleted while this while() loop process is on going,
         may cause program to crash
      */
      refined_Word = new char[strlen(current_Word) + 1](); // include 1 space for null character

      strcpy(refined_Word, current_Word);

      // check if this word is unique.
      // for the very first call, this will always return true since the word is the first word.
      is_newUnique = checkif_newUniq(refined_Word, word_Array, dyna_Size, array_Index);

      if(is_newUnique)
      {
         // expand array for every unique word as needed
         expand_Arr(word_Array, dyna_Size);

         array_Index = (dyna_Size - 1); // the index location for the new unique word

         word_Array[array_Index].the_Word = refined_Word; // point to refined_Word to prevent losing access to it.
         word_Array[array_Index].found_Count++; // update occurrence count
         word_Array[array_Index].string_Length = strlen(word_Array[array_Index].the_Word); // take string length
      }
      else
      {
         word_Array[array_Index].found_Count++; // just increase count of the word if not a new word.
         delete [] refined_Word; // deallocate memory
      }
      refined_Word = nullptr;

      // take next token
      current_Word = strtok(nullptr, " .,;!?");
   }

   delete [] copy_currentLine; // deallocate memory
   copy_currentLine = nullptr;

} // end of tokenize_Line()

// filter out the non-Alphabetic character(s)
void filter_Characters(char current_Word[])
{
   int index = 0; // to be used when modifying the current word.
   int not_Alpha = 0; // hold number of non-Alphabetic
   int word_Size = strlen(current_Word); // to be used for padding null character(s)

   // modify current word
   // remove any non-Alphabetic.
   // After this, there may be unneeded duplicate character(s)
   for(int i = 0; i < word_Size; i++)
   {
      if(isalpha(current_Word[i]))
      {
         current_Word[index] = tolower(current_Word[i]);
         index++; // keep track of next available index for char insertion
      }
      else
      {
         not_Alpha++; // keep track of number of non-Alphabetic
      }
   }

   //current_Word[word_Size - not_Alpha] = '\0'; // this may be enough. But will only insert a single '\0'
                                                 // and the rest would still be the duplicate character(s)
                                                 // uncomment if prefer this, then comment below for() loop

   // remove duplicate character(s) and pad it with bunch of null character(s)
   // just to be safe, and for the sake of the consistency
   // (word_Size - not_Alpha) will let us start at the first duplicate character
   for(int i = (word_Size - not_Alpha); i < word_Size; i++)
   {
      current_Word[i] = '\0';
   }

   // capitalize first letter
   current_Word[0] = toupper(current_Word[0]);

} // end of filter_Characters()

// check if the word is not in unique word array
bool checkif_newUniq(const char refined_Word[], const Unique_Word word_Array[], const int &dyna_Size, int &array_Index)
{
   // for() loop statement will not execute for the very first call and so will always return true.
   for(int i = 0; i < dyna_Size; i++)
   {
      if(strcmp(word_Array[i].the_Word, refined_Word) == 0) // match found! Quick! Return!
      {
         array_Index = i;
         return(false);
      }
   }

   // if the for() loop didn't execute, or finished executing without finding a match then word is new, return true
   return(true);

} // end of checkif_newUniq()

// expands array size as needed
void expand_Arr(Unique_Word *&word_Array, int &dyna_Size)
{
   Unique_Word *temp_Ptr = nullptr;

   temp_Ptr = word_Array; // point to current array to make sure it won't be lost during expansion
                          // for the very first expansion, temp_Ptr will still point at NULL.

   dyna_Size++; // increase the dyna_Size for the new array
   word_Array = new Unique_Word[dyna_Size](); // create new array of dyna_Size and initialize to prevent undefined behavior

   // copy old array to new array up until size - 1, the last new index will be reserved
   // when returned to caller function for new word insertion.
   // Will not execute for the very first expansion, nothing to copy.
   for(int i = 0; i < dyna_Size - 1; i++)
   {
      word_Array[i] = temp_Ptr[i];
   }

   if(temp_Ptr != nullptr)
   {
      delete [] temp_Ptr; // deallocate memory containing old array to prevent memory leak

      temp_Ptr = nullptr; // may be unnecessary since end of function destroys this variable
   }

} // end of expand_Arr()



// gather word information to be used for taking word statistics
void word_Info(const Unique_Word word_Array[], const int &dyna_Size, int &long_Loc, int &most_Loc, int &short_Count)
{
   int random_num;
   unsigned int seed = time(0);

   srand(seed);

   // this will go through the list
   for(int i = 0; i < dyna_Size; i++)
   {
      // find index of longest word in the array
      if(word_Array[i].string_Length > word_Array[long_Loc].string_Length)
      {
         long_Loc = i;
      }
      else if(word_Array[i].string_Length == word_Array[long_Loc].string_Length)
      {
         random_num = rand() % 2; // select number between 0 and 1;

         if(random_num == 1) // replace current long_Loc if met condition, else keep old long_Loc value
         {
            long_Loc = i;
         }
      }

      // find index of most occurring word in the array
      if(word_Array[i].found_Count > word_Array[most_Loc].found_Count)
      {
         most_Loc = i;
      }
      else if(word_Array[i].found_Count == word_Array[most_Loc].found_Count)
      {
         random_num = rand() % 2; // select number between 0 and 1;

         if(random_num == 1) // replace current most_Loc if met condition, else keep old most_Loc value
         {
            most_Loc = i;
         }
      }

      // count number of 1-3 character(s) word(s)
      if((word_Array[i].string_Length >= 1) && (word_Array[i].string_Length <= 3))
      {
         short_Count++;
      }
   }

} // end of word_Info()



/***** start of I/O functions implementations *****/

// display output to console
void display_Output(const Unique_Word word_Array[], const int &dyna_Size, const int &long_Loc, const int &most_Loc, const int &short_Count) // try const int
{
   // header for each column
   char header_1[] = "Word";
   char header_2[] = "Occurrence count";

   /*
   Note:
     - to be used for setting up fields width and borderlines, change it as needed.
     - border_1 is the total of all setw() values
     - border_2 is the total of all setw() values, but using the length of header_1
     - this is to prevent words with <= 3 char from messing up the border lines
     - try using only border_1 to see what I mean
   */
   unsigned int header1_Size = strlen(header_1);
   unsigned int header2_Size = strlen(header_2);
   unsigned int long_WordSize = word_Array[long_Loc].string_Length;
   unsigned int border_1 = (7 + long_WordSize + header2_Size);
   unsigned int border_2 = (7 + header1_Size + header2_Size);
   unsigned int final_Border = (long_WordSize > header1_Size) ? border_1 : border_2;
   unsigned int gap = strlen("List in sorted order: ");

   cout << "\nList in sorted order: " << "\n";
   cout << "\n";

   cout << setw(gap) << " ";
   for(int i = 0; i < final_Border; i++)
   {
      cout << "=";
   }

   cout << "\n";

   cout << setw(gap) << " ";
   cout << setw(2) << left << "|"
        << setw((long_WordSize > header1_Size) ? long_WordSize : header1_Size) << header_1
        << setw(3) << " |" << setw(header2_Size) << right << header_2 << setw(2)
        << "|" << "\n";

   cout << setw(gap) << " ";
   for(int i = 0; i < final_Border; i++)
   {
      cout << "=";
   }

   cout << "\n";

   for(int i = 0; i < dyna_Size; i++)
   {
      // try using only long_WordSize before word_Array[i].the_Word
      // words with <= 3 characters will mess up the border lines
      cout << setw(gap) << " ";
      cout << setw(2) << left << "|"
           << setw((long_WordSize > header1_Size) ? long_WordSize : header1_Size)
           << word_Array[i].the_Word << setw(3) << " |" << setw(header2_Size)
           << right << word_Array[i].found_Count << setw(2) << "|" << "\n";

      cout << setw(gap) << " ";
      for(int j = 0; j < final_Border ; j++)
      {
         // order of precedence matter!
         (((j == 0) || (j == (final_Border - 1))) && (i != (dyna_Size - 1))) ? cout << "|" : cout << "-";
      }

      cout << "\n";
   }

   cout << "\nLongest word is: " << word_Array[long_Loc].the_Word << "\n"
        << "Word that occurred most is: " << word_Array[most_Loc].the_Word << "\n"
        << "Number of short word(s) found is: " << short_Count << "\n";

} // end of display_Output()

// write to output file
// Note: the code is almost exactly the same as display_Output() function
void write_Fout(ofstream &fout, const Unique_Word word_Array[], const int &dyna_Size, const int &long_Loc, const int &most_Loc, const int &short_Count) // try const int&
{
   // header for each column
   char header_1[] = "Word";
   char header_2[] = "Occurrence count";

   /*
   Note:
     - to be used for setting up fields width and borderlines, change it as needed.
     - border_1 is the total of all setw() values
     - border_2 is the total of all setw() values, but using the length of header_1
     - this is to prevent words with <= 3 char from messing up the border lines
     - try using only border_1 to see what I mean
   */
   unsigned int header1_Size = strlen(header_1);
   unsigned int header2_Size = strlen(header_2);
   unsigned int long_WordSize = word_Array[long_Loc].string_Length;
   unsigned int border_1 = (7 + long_WordSize + header2_Size);
   unsigned int border_2 = (7 + header1_Size + header2_Size);
   unsigned int final_Border = (long_WordSize > header1_Size) ? border_1 : border_2;
   unsigned int gap = strlen("List in sorted order: ");

   fout << "\nList in sorted order: " << "\n";
   fout << "\n";

   fout << setw(gap) << " ";
   for(int i = 0; i < final_Border; i++)
   {
      fout << "=";
   }

   fout << "\n";

   fout << setw(gap) << " ";
   fout << setw(2) << left << "|"
        << setw((long_WordSize > header1_Size) ? long_WordSize : header1_Size) << header_1
        << setw(3) << " |" << setw(header2_Size) << right << header_2 << setw(2)
        << "|" << "\n";

   fout << setw(gap) << " ";
   for(int i = 0; i < final_Border; i++)
   {
      fout << "=";
   }

   fout << "\n";

   for(int i = 0; i < dyna_Size; i++)
   {
      // try using only long_WordSize before word_Array[i].the_Word
      // words with >= 3 characters will mess up the border lines
      fout << setw(gap) << " ";
      fout << setw(2) << left << "|"
           << setw((long_WordSize > header1_Size) ? long_WordSize : header1_Size)
           << word_Array[i].the_Word << setw(3) << " |" << setw(header2_Size)
           << right << word_Array[i].found_Count << setw(2) << "|" << "\n";

      fout << setw(gap) << " ";
      for(int j = 0; j < final_Border; j++)
      {
         (((j == 0) || (j == (final_Border - 1))) && (i != (dyna_Size - 1))) ? fout << "|" : fout << "-";
      }

      fout << "\n";
   }

   fout << "\nLongest word is: " << word_Array[long_Loc].the_Word << "\n"
        << "Word that occurred most is: " << word_Array[most_Loc].the_Word << "\n"
        << "Number of short word(s) found is: " << short_Count << "\n";

} // end of write_Fout()

/***** end of I/O functions implementations *****/


/****** start of sorting algorithms implementation *****/

// switch elements values
// used by QuickSort() and BubbleSort()
void swap_Element(Unique_Word &word1, Unique_Word &word2)
{
   Unique_Word temp_var = word1;
   word1 = word2;
   word2 = temp_var;

} // end of swap_Element()

// for QuickSort()
int partition(Unique_Word word_Array[], int low, int high)
{
    char* pivot = word_Array[high].the_Word;

    int LEFT = low;
    int RIGHT = high;


    while(LEFT < RIGHT)
    {

        while((strcmp(word_Array[LEFT].the_Word, pivot) < 0) && LEFT != RIGHT)
        {
           ++LEFT;
        }

        while((strcmp(word_Array[RIGHT].the_Word, pivot) > 0) && RIGHT != LEFT)
        {
            --RIGHT;
        }

        if(strcmp(word_Array[RIGHT].the_Word, word_Array[LEFT].the_Word) == 0)
        {
            ++LEFT;
            continue;
        }

        if(LEFT != RIGHT)
        {
            swap_Element(word_Array[LEFT], word_Array[RIGHT]);
        }
    }

    return(LEFT - 1);

} // end of partition()

void QuickSort(Unique_Word word_Array[], int low, int high)
{
    if(low < high)
    {
        int pivotLoc = partition(word_Array, low, high);

        QuickSort(word_Array, low, pivotLoc - 1);
        QuickSort(word_Array, pivotLoc + 1, high);
    }
} // end of QuickSort()



void BubbleSort(Unique_Word word_Array[], const int &dyna_Size)
{
   bool no_Swap;
   no_Swap = true; // set to true so the loop will execute

   for(int i = 0; ((i < (dyna_Size - 1)) && (no_Swap)); i++)
   {
      for(int j = 0; j < ((dyna_Size - 1) - i); j++) // -i shrink the comparison space, no need to check elements to the right
      {
         if(strcmp(word_Array[j].the_Word, word_Array[j+1].the_Word) > 0)
         {
            swap_Element(word_Array[j], word_Array[j+1]);
            no_Swap = false;
         }
      }

      // if no swap was made after the latest pass, break and return to caller function
      if(no_Swap)
      {
         break;
      }
      else // if swap was made, make another pass.
      {
         no_Swap = true;
      }
   }

} // end of BubbleSort()

/****** end of sorting algorithms implementation *****/

