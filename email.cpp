//Objective: Prompt the user for the input and output filenames, open and read the input file, and output to the console each valid email found
//Name: Roxanne McKeever 1995683
//Course: COMSC-110-1199
//Compiler: TDM MinGW
//Editor: MS NotePad

//libraries
#include <deque>    //collection lists
#include <fstream>  //file input 
#include <iostream> // user IO
#include <string> //string 
using namespace std;


//Programmer defined data types
struct VEmail
{
  string email;  // a valid email from the input file
};

//Special compiler dependent definitions
//NONE

//global constants/variables
//NONE

//Programmer defined functions

void introduction(string obj, string ins);               // user introduction
void readFile(deque<VEmail>& vEList, string iFN);                               // reads file and calls process line to output to console
int processLine(deque<VEmail>& vEList, string lFF);                             //goes through each line and outputs it if it has a @ symbol, for every @ symbol found

string getFileName(string fileType, string dFileName);   // sets the file name
bool isValidEmailCharacter(char c);                      // checks to see if current letter is a valid character

void isDuplicate(deque<VEmail>& vEList, string anE);          //checks for duplicate emails in file
string changeCase(string s);                             //converts string to all uppercase for duplicate comparison 

void sortCollection(deque<VEmail>& vEList);              //sorts the list of emails

//main program
int main()
{
  //Data
  string objective = "Prompt the user for the input and output filenames, open and read the input file, and output to the console each valid email found.\n"; //program objective
  string instructions = "You will be prompted to enter both a input filename and an output file name. If you would prefer to use the dafault filenames, please just press the ENTER key.";     //user instructions
  string fileType = "Input";  //will chose which filename to output to console 
  string dFileName; //default file name (input: fileContainingEmails.txt, output: copyPasteMyEmails.txt)
  string enterKey;  //checks to see if only enter key was pressed to choose default filename
  string iFileName; //input file name chosen by the user unless default filename is chosen by pressing enter
  string oFileName; //output filename chosen by the user unless default filename is chosen by pressing enter 

  deque<VEmail> vEList;
  ofstream fout;

  // user introduction
  introduction(objective, instructions);
  
  //set input filename and output to console
  dFileName = "fileContainingEmails.txt";
  iFileName = getFileName(fileType, dFileName);
  
  fileType = "Output";

  // set output filename and output to console
  if (dFileName == iFileName)
  {
    dFileName = "copyPasteMyEmails.txt";
  }  
  else 
  {
    dFileName = iFileName;
  } 
  oFileName = getFileName(fileType, dFileName);
  
  // outputs filenames
  cout << "Input filename:" << iFileName << endl;
  cout << "Output filename:" << oFileName << endl;
  cout << "Press ENTER key to continue..." << endl;
  getline(cin, enterKey);

  //reads file and outputs line one at a time for each @ found
  readFile(vEList, iFileName);

  //outputs valid emails to output file
  fout.open(oFileName.c_str());
  for(int iFList = 0; iFList < vEList.size(); iFList++)
  {
    fout << vEList[iFList].email << "; "; 
  } 

}//main

// user introduction
void introduction(string obj, string ins)
{
  //data
  //obj is the program objective
  //ins is the user instructions
   
  //output user introduction
  cout << "Objective: This program will " << obj; 
  cout << "Programmer: Roxanne McKeever\n"; 
  cout << "Editor(s) used: Notepad\n"; 
  cout << "Compiler(s) used: TDM MinGW\n"; 
  cout << "File: " << __FILE__ << endl; 
  cout << "Complied: " << __DATE__ << " at " << __TIME__ << endl; 
  cout << "Instructions:"<< ins << endl;

}// introduction


// setting filenames
string getFileName(string fileType, string dFileName)
{
  //data 
  string input; //console input
 
  do
  {
    cout << "Please enter " << fileType << " filename [must end in .txt, default filename: " << dFileName << "]" << endl;
    getline(cin, input);
    if (input.length() == 0)
    {
       input = dFileName;
    }
   
    if (input.length() < 5 || input[input.length() - 1] != 't' || input[input.length() - 2]  != 'x' || input[input.length() - 3] != 't' || input[input.length() - 4] != '.')
    {
      cout << "filename must be at least 5 characters and end in .txt" << endl;
    }
  } while (input.length() < 5 || input[input.length() - 1] != 't' || input[input.length() - 2]  != 'x' || input[input.length() - 3] != 't' || input[input.length() - 4] != '.');

  return input;
}// getFileName

//reading file and passing to processLine
void readFile(deque<VEmail>& vEList, string iFN)
{
  //data
  string lineFromFile;
  ifstream fin;
  int nLines = 0;
  int nEmails = 0;
  
  //opens file and processes each line
  fin.open(iFN.c_str());
  if (!fin.good()) throw "I/O error";
  while (fin.good())
  {
    getline(fin, lineFromFile);
    nEmails = nEmails + processLine(vEList, lineFromFile);

    if (lineFromFile.length() > 0)
    {
      nLines = nLines + 1;
    }
  }
    
  //sorts collection of valid emails
  sortCollection(vEList);

  if (nLines < 1 )
  {
    cout << "Empty file was input" << endl;
  } 
  else if (vEList.size() > 0) 
  {
    for (int i = 0; i < vEList.size(); i++)
    {
      cout << vEList[i].email << ";" << endl;
    }
    
    cout << "Valid emails found: " << vEList.size() << endl;
  }
  else
  {
    cout << "No valid emails were found" << endl;
  }
  fin.close();

}//readFile

//processes line for valid emails
int processLine(deque<VEmail>& vEList, string lFF)
{
  //data
  string anEmail;
  int i = 0;
  int s = 0;
  int e = 0;
  int nEmails = 0;
  bool hasDot = false;
  int dotPos;

  for(i = 0; i <= lFF.length(); i++)
  {

    if (lFF[i] == '@')
    {

      for (s = i; s >= 0;)
      {
        s = s - 1;
        if (!isValidEmailCharacter(lFF[s]))
        {
          s = s + 1;
          break;
        }   
      } //for s loop 
      
      hasDot = false;
      for (e = i; i <= lFF.length(); )
      {
        e = e + 1;
        if (lFF[e] == '.' && !hasDot)
        {
          dotPos = e;
          hasDot = true;
        }
        if (!isValidEmailCharacter(lFF[e]))
        {
          break;
          e = e + 1;
        }   
      }//for e loop

      if(s < i && e > i && hasDot && dotPos > i + 1)
      {
        //extracts email from string
        anEmail = lFF.substr(s, e - s);

        //check for duplicates here
        isDuplicate(vEList, anEmail);
        nEmails++;
      }
        
    }//if i loop
    
  }//for i loop
  return nEmails;

}//processLine

//checks for valid characters
bool isValidEmailCharacter(char c)
{
  //data
  bool iVEC = false;

  if (c == 43 || c == 45 || c == 46 || c == 95)
  { 
    iVEC = true;
  }
  
  else if (c >= 48 && c <= 57)
  {
    iVEC = true;
  }
 
  else if (c >= 65 && c <= 90 || c >= 97 && c <= 122)
  {
    iVEC = true;
  }
  
   return iVEC;
}

//checks for duplicates
void isDuplicate(deque<VEmail>& vEList, string anE)
{
  //Data
  // aE is anEmail
  string tempCaseAnEmail;       //holds the uppercase version of anEmail
  string tempCaseListEmail;     //holds the uppercase version of an email from the current index of the valid email list
  bool isDup = false;           //if false will add the email to valid email list
  VEmail aVEmail;               //valid email from input file added to list
  
  //changes an email to uppercase for comparison
  tempCaseAnEmail = changeCase(anE);

  //traverses the collection to see if there is a duplicate
  for (int iDup = 0; iDup < vEList.size(); iDup++)
  {
    tempCaseListEmail = changeCase(vEList[iDup].email);
    if (tempCaseAnEmail == tempCaseListEmail)
    {
      isDup = true;
    }
  }  
  
  //if not duplicate add to the valid email list
  if(!isDup)
  {
    aVEmail.email = anE;
    vEList.push_back(aVEmail);
  }
}//isDuplicate

//changes cases from lower to upper for checking for duplicates
string changeCase(string s)
{

  for (int iCheck = 0; iCheck < s.length(); iCheck++)
  {
    s[iCheck] = toupper(s[iCheck]);
  }  

  return s;
}//changCase

//sorts collection of valid emails alphabetically 
void sortCollection(deque<VEmail>& vEList)
{
  //data
  int i; //loop index variable (outer loop)
  int j; //loop index variable (inner loop)
  VEmail temp; //holds an email temporarily during sort swap

  for (i = 0; i < vEList.size(); i++)
  {
    for(j = i + 1; j < vEList.size(); j++)
    {
      if(vEList[i].email > vEList[j].email)
      {
        temp = vEList[i];
        vEList[i] = vEList[j];
        vEList[j] = temp;
      }//if
    }//for j
  } //for i
}//sortCollection

