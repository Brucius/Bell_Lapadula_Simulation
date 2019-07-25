// The MIT License (MIT)

// Copyright (c) 2019 OAKER MIN

//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.

/* code */

#include <iostream>
#include <string>
#include <fstream>
#include "md5.h"
#include <time.h>
#include <random>
#include <cstring>
#include <algorithm>
#include <cctype>
#include <sstream>
using namespace std;

string uName;
string pW;
string confirmPw;
string storedUName;
string stored_pW;
string salt;
string saltedpW;
string salteduName;
string hashSaltedPw;
string uClearance;
string loginSalt;
string loginHashSaltPass;
string fileName;
string fileClearence;
string fileNameTrimmed;
string fileClearanceTrimmed;
string storedShadow;
string storedClearence;
vector<string> fileStore;

// Function Prototypes here
int mainMenu(int);
void createAccount();
// all the subfunctions of create account here
string saltGenerator(string &, string &);
void writeToSaltFile();
string userClearance();
void writeToShadowFile();
// void printStrongNess(string &);

// Login function
void login();
void uNameLoginCheck();
string generateLoginPwSaltHash(string &, string &);
void checkPassSaltHash();
void fileSystem();
void retriveFileStore(vector<string> &);
void createFile();
void readFile();
void writeFile();
void listFile();
void saveFile();
void promptExit();

/*************************************************  START OF MAIN FUNCTION *************************************************/
int main(int argc, char *argv[])
{
  if (argc == 2 && string(argv[1]) == "-i")
  {
    cout << "Account Creation.\n";
    cout << "This is a test = " << md5("This is a test") << "\n";
    createAccount();
  }
  else if (argc == 1)
  {
    cout << "Account Login.\n";
    cout << "This is a test = " << md5("This is a test") << "\n";
    login();
  }
  return 0;
}
/*************************************************  END OF MAIN FUNCTION *************************************************/

/************************************************* Function Implementations *************************************************/

/* Creating account function. This function will check for existing user and password upon user input to create an account. 
This function can be invoked by FileSystem -i initialization */
void createAccount()
{
  cout << "Username : ";
  cin >> uName;
  cin.ignore();
  transform(uName.begin(), uName.end(), uName.begin(), ptr_fun<int, int>(tolower));
  string line;
  ifstream saltFile;
  bool found = false;

  saltFile.open("salt.txt");
  while (saltFile.good())
  {
    getline(saltFile, line);
    size_t pos = line.find(":");
    storedUName = line.substr(0, pos);
    if (storedUName == uName)
    {
      found = true;
      break;
    }
  }
  saltFile.close();

  if (found == false)
  {
  again:
    do
    {
      cout << "Password : ";
      cin >> pW;
      cin.ignore();
      cout << "Confirm Password : ";
      cin >> confirmPw;
      cin.ignore();
      if (pW != confirmPw)
      {
        cout << "The passwords do not match. Please try again\n";
      }
    } while (pW != confirmPw);
    // transform(pW.begin(), pW.end(), pW.begin(), ptr_fun<int, int>(tolower));
    // saltGenerator(uName, pW);
    // userClearance();
    // writeToSaltFile();
    // writeToShadowFile();
    // printStrongNess(pW);
    int n = pW.length();
    // Checking lower alphabet in string
    bool hasLower = false, hasUpper = false;
    bool hasDigit = false, specialChar = false;
    string normalChars = "abcdefghijklmnopqrstu"
                         "vwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ";

    for (int i = 0; i < n; i++)
    {
      if (islower(pW[i]))
        hasLower = true;
      if (isupper(pW[i]))
        hasUpper = true;
      if (isdigit(pW[i]))
        hasDigit = true;

      size_t special = pW.find_first_not_of(normalChars);
      if (special != string::npos)
        specialChar = true;
    }
    // Strength of password
    if (hasLower && hasUpper && hasDigit &&
        specialChar && (n >= 8))
    {
      cout << "Password strength strong, proceeding to create account." << endl;
      saltGenerator(uName, pW);
      userClearance();
      writeToSaltFile();
      writeToShadowFile();
    }
    else
    {
      cout << "Password strength weak, pleaset try again." << endl;
      goto again;
    }
  }
  else
  {
    cout << "Username already exists. Exiting Program.\n";
  }
}

/* This function generates 8 random numbers and append it behind both username and password. */
string saltGenerator(string &, string &)
{
  srand(time(NULL));
  for (int i = 0; i < 8; i++)
  {
    int digit = rand() % 10;
    salt += to_string(digit);
  }
  salteduName = uName + ":" + salt;
  saltedpW = pW + salt;
  hashSaltedPw = md5(saltedpW);
  return salteduName;
  return hashSaltedPw;
}

/* This function creates the username with salt value into salt.txt file. */
void writeToSaltFile()
{
  //Bob:12345678
  //Username:Salt
  ofstream writeSalt;
  writeSalt.open("salt.txt", ios::app);
  writeSalt << salteduName + "\n";
  writeSalt.close();
  cout << "Successfully written into salt file : " << salteduName << "\n";
}

/* This function hashes salted password and write to shadow.txt file */
void writeToShadowFile()
{
  //Bob:dd2da44f4437d529a80809932cb3da83:1
  //Username:hash:AccessControl
  ofstream shadowFile;
  shadowFile.open("shadow.txt", ios::app);
  string saltHash = uName + ":" + hashSaltedPw + ":" + uClearance + "\n";
  shadowFile << saltHash;
  shadowFile.close();
  cout << "Successfully written into shadow file : " << saltHash;
}

/* This function checks for valid security clearance level */
string userClearance()
{
again:
  cout << "User clearance (0 or 1 or 2 or 3) : ";
  cin >> uClearance;
  cin.ignore();
  if (uClearance != "0" && uClearance != "1" && uClearance != "2" && uClearance != "3")
  {
    cout << "Invalid clearance input. Please try again! \n";
    goto again;
  }
  else
  {
    cout << "User clearance is : " << uClearance << "\n";
  }
  return uClearance;
}

/* This function checks for existing username and password hashes from user input to validate successful logins.
Can be invoked by FileSystem. */
void login()
{
  string line;

  cout << "Username : ";
  cin >> uName;
  cin.ignore();
  transform(uName.begin(), uName.end(), uName.begin(), ptr_fun<int, int>(tolower));
  cout << "Password : ";
  cin >> pW;
  cin.ignore();
  cout << "\n\n";
  //function to generate salt and hash for PW
  ifstream saltFile;
  bool found = false;
  saltFile.open("salt.txt");
  while (saltFile.good())
  {
    getline(saltFile, line);
    size_t pos = line.find(":");
    storedUName = line.substr(0, pos);
    if (storedUName == uName)
    {
      loginSalt = line.substr(pos + 1);
      //cout << loginSalt;
      break;
    }
  }
  saltFile.close();
  retriveFileStore(fileStore);
  generateLoginPwSaltHash(pW, loginSalt);
  checkPassSaltHash();
}

/* This function reads in the existing file store from the fileSimulation.txt and stores it in a vector */
void retriveFileStore(vector<string> &fileVector)
{
  string line;
  ifstream file;
  file.open("fileSimulation.txt");
  while (file.good())
  {
    getline(file, line);
    fileVector.push_back(line);
  }
  file.close();
}

/* This function generates login hash from user input to check for existing hasehd salted password. */
string generateLoginPwSaltHash(string &, string &)
{
  string loginSaltedPass = pW + loginSalt;
  loginHashSaltPass = md5(loginSaltedPass);
  return loginHashSaltPass;
}

/* This function checks for the hashed salted password returned from the generateLoginPwSaltHash */
void checkPassSaltHash()
{
  string line;
  bool found = false;

  ifstream shadowFile;
  shadowFile.open("shadow.txt");
  while (shadowFile.good())
  {
    getline(shadowFile, line);
    size_t pos = line.find(":");
    storedShadow = line.substr(pos + 1);
    size_t pos2 = storedShadow.find(":");
    storedShadow = storedShadow.substr(0, pos2);

    storedClearence = line.substr(pos + 1);
    size_t pos3 = storedClearence.find(":");
    storedClearence = storedClearence.substr(pos3 + 1);
    // cout << "\n" << storedClearence;

    if (loginHashSaltPass == storedShadow)
    {
      found = true;
      break;
    }
  }
  shadowFile.close();

  if (found == true)
  {
    cout << uName << " found in salt.txt\n";
    cout << "salt retrieved : " << loginSalt << "\n";
    cout << "hashing......\n";
    cout << "hash value : " << loginHashSaltPass << "\n";
    cout << "Authentication for user " << uName << " complete.\n";
    cout << "The clearence for " << uName << " is " << storedClearence << "\n\n\n";
    fileSystem();
  }
  else
  {
    cout << "Wrong Username or Password entered. Exiting Program. Please try again.\n";
  }
}

/* Major main menu function for File System after successful login. */
void fileSystem()
{
  string exitProgram = "";
  do
  {
    char choice;
    cout << "Options: (C)reate, (R)ead, (W)rite, (L)ist, (S)ave or (E)xit.\n";
    cout << "Please enter your option : ";
    cin >> choice;
    cin.ignore();
    choice = toupper(choice);
    switch (choice)
    {
    case 'C':
      for (int i = 0; i < fileStore.size(); i++)
      {
        cout << fileStore[i] << "\n";
      }
      createFile();
      for (int i = 0; i < fileStore.size(); i++)
      {
        cout << fileStore[i] << "\n";
      }
      break;

    case 'R':
      readFile();
      break;

    case 'W':
      writeFile();
      break;

    case 'L':
      listFile();
      break;

    case 'S':
      saveFile();
      break;

    case 'E':
      exitProgram = 'E';
      promptExit();
    }
  } while (exitProgram == "");
}

/* Implementation of create file record with no Write Down BLP model */
void createFile()
{
  string line;
  bool found = false;
  ifstream inFile;

  cout << "Filename : ";
  cin >> fileName;
  cin.ignore();

  inFile.open("fileSimulation.txt");
  while (inFile.good())
  {
    getline(inFile, line);
    size_t pos = line.find(":");
    fileNameTrimmed = line.substr(0, pos);
    fileClearanceTrimmed = line.substr(pos + 1);
    if (fileNameTrimmed == fileName)
    {
      found = true;
      break;
    }
  }
  inFile.close();

  if (found == true)
  {
    cout << "File already exists. Please try creating again.\n\n";
  }
  else
  {
  again:
    cout << "Security level (0 or 1 or 2 or 3) : ";
    cin >> fileClearence;
    cin.ignore();
    if (storedClearence <= fileClearence)
    {
      fileStore.push_back(fileName + ":" + fileClearence);
      cout << "Successfully Created File!";
    }
    else if (storedClearence > fileClearence)
    {
      cout << "Your clearance level " << storedClearence << " is not allowed to write a file of " << fileClearence << " level.\n";
      goto again;
    }
  }
}

/* Implementation of no read up BLP model for read file. */
void readFile()
{
  string line;
  bool found = false;
  ifstream inFile;

  cout << "\nPlease enter Filename to read : ";
  cin >> fileName;
  cin.ignore();
  transform(fileName.begin(), fileName.end(), fileName.begin(), ptr_fun<int, int>(tolower));
  for (int i = 0; i < fileStore.size(); i++)
  {
    // cout << fileStore[i] << "\n";
    size_t pos = fileStore[i].find(":");
    fileNameTrimmed = fileStore[i].substr(0, pos); // which one to use?
    fileClearanceTrimmed = fileStore[i].substr(pos + 1);
    if (fileNameTrimmed == fileName)
    {
      cout << fileNameTrimmed << "\n";
      cout << fileClearanceTrimmed << "\n";
      found = true;
      break;
    }
  }

  if (found == true)
  {
    if (storedClearence >= fileClearanceTrimmed)
    {
      cout << "You have successfully read the file!\n\n";
    }
    else if (storedClearence < fileClearanceTrimmed)
    {
      cout << "You do not have necessary permission to read the file!\n\n";
    }
  }
  else
  {
    cout << "This file does not exist. Please input again.\n\n";
  }
}

/* Implementation of no Write Down BLP model */
void writeFile()
{
  string line;
  bool found = false;
  ifstream inFile;

  cout << "\nPlease enter Filename to write : ";
  cin >> fileName;
  cin.ignore();
  transform(fileName.begin(), fileName.end(), fileName.begin(), ptr_fun<int, int>(tolower));
  for (int i = 0; i < fileStore.size(); i++)
  {
    // cout << fileStore[i] << "\n";
    size_t pos = fileStore[i].find(":");
    fileNameTrimmed = fileStore[i].substr(0, pos);
    fileClearanceTrimmed = fileStore[i].substr(pos + 1);
    if (fileNameTrimmed == fileName)
    {
      cout << fileNameTrimmed << "\n";
      cout << fileClearanceTrimmed << "\n";
      found = true;
      break;
    }
  }
  if (found == true)
  {
    if (storedClearence > fileClearanceTrimmed)
    {
      cout << "You are not allowed to write to the file\n\n";
    }
    else if (storedClearence <= fileClearanceTrimmed)
    {
      cout << "You have successfully written to the file!\n\n";
    }
  }
  else
  {
    cout << "This file does not exist. Please input again\n\n";
  }
}

/* Implementation of file listing */
void listFile()
{
  string line;
  bool found = false;
  ifstream inFile;
  cout << "\n############### List of existing files and clearance ###############\n";
  for (int i = 0; i < fileStore.size(); i++)
  {
    cout << fileStore[i] << "\n";
  }
}

/* Implementation of save file function */
void saveFile()
{
  ofstream myFile;
  myFile.open("FileSimulation.txt", ios::out | ios::trunc);
  for (int i = 0; i < fileStore.size(); i++)
  {
    myFile << fileStore[i] << "\n";
  }
  myFile.close();
  cout << "Successfully saved file!\n";
  cout << "current number of files : " << fileStore.size();
}

/* Implementation of exit function */
void promptExit()
{
  char uInput;
  cout << "Shut down the FileSystem? (Y)es or (N)o : ";
  cin >> uInput;
  cin.ignore();
  uInput = toupper(uInput);
  if (uInput == 'Y')
  {
    cout << "Exited File System Successfully.\n";
  }
  else
  {
    cout << "Going back to Main Menu.\n\n";
    fileSystem();
  }
}