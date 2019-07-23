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

// Function Prototypes here
int mainMenu(int);
void createAccount();
// all the subfunctions of create account here
string saltGenerator(string &, string &);
void writeToSaltFile();
string userClearance();
void writeToShadowFile();
void printStrongNess(string &);

// Login function
void login();
void uNameLoginCheck();
string generateLoginPwSaltHash(string &, string &);
void checkPassSaltHash();
void fileSystem();
void createFile();
void readFile();
void writeFile();
void listFile();
void promptExit();

/*************************************************  START OF MAIN FUNCTION *************************************************/
int main(int argc, char *argv[])
{
  if (argc == 2 && string(argv[1]) == "-i")
  {
    cout << "Account Creation.\n";
    createAccount();
  }
  else if (argc == 1)
  {
    cout << "Account Login.\n";
    login();
  }
  return 0;
}
/*************************************************  END OF MAIN FUNCTION *************************************************/

/************************************************* Function Implementations *************************************************/
void createAccount()
{
  cout << "Username : ";
  cin >> uName;
  cin.ignore();
  transform(uName.begin(),uName.end(),uName.begin(), ptr_fun <int,int> (tolower));
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
    transform(pW.begin(),pW.end(),pW.begin(), ptr_fun <int,int> (tolower));
    saltGenerator(uName, pW);
    userClearance();
    writeToSaltFile();
    writeToShadowFile();
    printStrongNess(pW);
  }
  else
  {
    cout << "Username already exists. Exiting Program.\n";
  }
}

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

void printStrongNess(string &input)
{
  int n = input.length();

  // Checking lower alphabet in string
  bool hasLower = false, hasUpper = false;
  bool hasDigit = false, specialChar = false;
  string normalChars = "abcdefghijklmnopqrstu"
                       "vwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ";

  for (int i = 0; i < n; i++)
  {
    if (islower(input[i]))
      hasLower = true;
    if (isupper(input[i]))
      hasUpper = true;
    if (isdigit(input[i]))
      hasDigit = true;

    size_t special = input.find_first_not_of(normalChars);
    if (special != string::npos)
      specialChar = true;
  }

  // Strength of password
  cout << "Strength of password : ";
  if (hasLower && hasUpper && hasDigit &&
      specialChar && (n >= 8))
    cout << "Strong" << endl;
  else if ((hasLower || hasUpper) &&
           specialChar && (n >= 6))
    cout << "Moderate" << endl;
  else
    cout << "Weak" << endl;
}

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

void login()
{
  string line;

  cout << "Username : ";
  cin >> uName;
  cin.ignore();
  transform(uName.begin(),uName.end(),uName.begin(), ptr_fun <int,int> (tolower));
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

  generateLoginPwSaltHash(pW, loginSalt);
  checkPassSaltHash();
}

string generateLoginPwSaltHash(string &, string &)
{
  string loginSaltedPass = pW + loginSalt;
  loginHashSaltPass = md5(loginSaltedPass);
  return loginHashSaltPass;
}

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
      // cout << "this is hashed " << loginHashSaltPass << "\n";
      // cout << "\n this is stored " << storedShadow << "\n";
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
      createFile();
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
      cout << "Save file place holder.\n";
      // function()
      break;

    case 'E':
      exitProgram = 'E';
      promptExit();
    }
  } while (exitProgram == "");
}

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
    if (fileNameTrimmed == fileName || fileClearanceTrimmed == fileClearence)
    {
      cout << fileNameTrimmed << "\n";
      cout << fileClearanceTrimmed << "\n";
      found = true;
      break;
    }
  }
  inFile.close();

  if (found == true)
  {
    cout << "File already exists. Please try creating again.\n\n";
  }
  else if (found == false)
  {
    cout << "Security level (0 or 1 or 2 or 3) : ";
    cin >> fileClearence;
    cin.ignore();
    ofstream writeFile;
    writeFile.open("fileSimulation.txt", ios::app);
    string writer = fileName + ":" + fileClearence + "\n";
    writeFile << writer;
    writeFile.close();
    cout << "Successfully written into file : " << writer << "\n\n";
  }
}

void readFile()
{
  string line;
  bool found = false;
  ifstream inFile;

  cout << "\nPlease enter Filename to read : ";
  cin >> fileName;
  cin.ignore();
  transform(fileName.begin(), fileName.end(), fileName.begin(), ptr_fun<int, int>(tolower));
  inFile.open("fileSimulation.txt");
  while (inFile.good())
  {
    getline(inFile, line);
    size_t pos = line.find(":");
    fileNameTrimmed = line.substr(0, pos);
    fileClearanceTrimmed = line.substr(pos + 1);
    if (fileNameTrimmed == fileName)
    {
      cout << fileNameTrimmed << "\n";
      cout << fileClearanceTrimmed << "\n";
      found = true;
      break;
    }
  }
  inFile.close();
  if (found == true)
  {
    if (storedClearence >= fileClearanceTrimmed)
    {
      // cout << "user clearance : " << storedClearence << "\n";
      // cout << "file clearance : " << fileClearanceTrimmed << "\n";
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

void writeFile()
{
  string line;
  bool found = false;
  ifstream inFile;

  cout << "\nPlease enter Filename to read : ";
  cin >> fileName;
  cin.ignore();
  transform(fileName.begin(), fileName.end(), fileName.begin(), ptr_fun<int, int>(tolower));
  inFile.open("fileSimulation.txt");
  while (inFile.good())
  {
    getline(inFile, line);
    size_t pos = line.find(":");
    fileNameTrimmed = line.substr(0, pos);
    fileClearanceTrimmed = line.substr(pos + 1);
    if (fileNameTrimmed == fileName)
    {
      // cout << fileNameTrimmed << "\n";
      // cout << fileClearanceTrimmed << "\n";
      found = true;
      break;
    }
  }
  inFile.close();
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

void listFile()
{
  string line;
  bool found = false;
  ifstream inFile;
  cout << "\n############### List of existing files ###############\n";
  inFile.open("fileSimulation.txt");
  while (inFile.good())
  {
    getline(inFile, line);
    size_t pos = line.find(":");
    fileNameTrimmed = line.substr(0, pos);
    cout << fileNameTrimmed << "\n";
}
inFile.close();
}

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