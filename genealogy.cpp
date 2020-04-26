#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include<bits/stdc++.h>
#include <sstream>

using namespace std;

const int family_person_size = 100;
const int child_size = 10;

struct Family{
        int data; // value for "n"
        struct Person *husband, *wife;
        struct vector<Person*> child;
};

struct Person{
        int data; // value for "n"
        struct Family *parents, *marriage;
};

void relateHelper(Person person[], int person1, int person2, bool foundStatus);

void initilizeTree(Person person[], Family family[]){
        Person *personPtr;
        Family *familyPtr;

        for(int i = 1; i < family_person_size; i++){
                personPtr = &person[i];
                personPtr->data = i;
                personPtr->parents = NULL;
                personPtr->marriage = NULL;
        }

        for(int i = 1; i < family_person_size; i++){
                familyPtr = &family[i];
                familyPtr->data = i;
                familyPtr->husband = NULL;
                familyPtr->wife = NULL;
                for(int childIndex = 0; childIndex < child_size; childIndex++){
                        familyPtr->child.push_back(NULL);
                }
        }
}

void buildPerson(Person person[], Family family[], vector<string> command){
        int personID = stoi(command[1]);
        int parentsID = stoi(command[3]);
        int marriageID = stoi(command[5]);

        Person *personPtr = &person[personID];
        Family *parentsPtr = &family[parentsID];
        Family *marriagePtr = &family[marriageID];

        personPtr->parents = parentsPtr;
        personPtr->marriage = marriagePtr;

        cout << "Person " << personPtr->data << " has parents " << personPtr->parents->data << " and is married in family " << personPtr->marriage->data << "." << endl;
}

void buildFamily(Person person[], Family family[], vector<string> command){
        int familyID = stoi(command[1]);
        int husbandID = stoi(command[3]);
        int wifeID = stoi(command[5]);
        int childID;

        Family *familyPtr = &family[familyID];
        Person *husbandPtr = &person[husbandID];
        Person *wifePtr = &person[wifeID];
        Person *childPtr;

        familyPtr->husband = husbandPtr;
        familyPtr->wife = wifePtr;

        for(int i = 7; i < command.size(); i+=2){
                childID = stoi(command[i]);
                childPtr = &person[childID];
                familyPtr->child.push_back(childPtr);
        }

        cout << "Family " << familyPtr->data << " has husband " << familyPtr->husband->data << ", wife " << familyPtr->wife->data << " and children";

        string childString;
        string wsa = " "; // white space adder = wsa
        for(int i = 0; i < familyPtr->child.size(); i++){
                if(familyPtr->child[i] != NULL){
                        childString.append(wsa);
                        childString.append(to_string(familyPtr->child[i]->data));
                }
        }
        cout << childString << "." << endl;
}

void verify(Person person[], Family family[], vector<string> personQueue, vector<string> familyQueue){
        int personID = 0;
        int familyID = 0;

        for(int personIndex = 0; personIndex < personQueue.size(); personIndex++){ // first, if a person has a parents pointer, check for a child back pointer:
                personID = stoi(personQueue[personIndex]);
                Person *personPtr = &person[personID];
                bool childPtrFound; // boolean to determine if child back pointer was found in the family's vector of children

                if(personPtr->parents != NULL && personPtr->parents->data != 0){
                        childPtrFound = false;
                        Family *potentialParents = personPtr->parents; // set potentialParents pointer to the initial person's family (as they are the parents, technically)
                        for(int childIndex = 0; childIndex < potentialParents->child.size(); childIndex++){ // for each child in the child vector
                                if(potentialParents->child[childIndex] != NULL){
                                        if(potentialParents->child[childIndex]->data == personPtr->data)
                                                childPtrFound = true; // set childPtrFound if child is found
                                }
                        }
                }
                if(!childPtrFound) // print if child not found
                        cout << "Person " << personPtr->data << " points to parent family " << personPtr->parents->data << ", but there is no back pointer." << endl;
        }

        for(int familyIndex = 0; familyIndex < familyQueue.size(); familyIndex++){ // for each family, apply the same logic from above, but with no boolean values
                familyID =  stoi(familyQueue[familyIndex]);
                Family *familyPtr = &family[familyID];
                if(familyPtr->wife != NULL){
                        Person *potentialWife = familyPtr->wife;
                        if(potentialWife->marriage == NULL && potentialWife->data != 0){
                                cout << "Family " << familyPtr->data << " points to wife person " << potentialWife->data << " but there is no back pointer." << endl;
                        }else if(potentialWife->data != 0 && potentialWife->marriage->data != familyPtr->data){
                                cout << "Family " << familyPtr->data << " points to wife person " << potentialWife->data << " but there is no back pointer." << endl;
                        }
                }

                if(familyPtr->husband != NULL){
                        Person *potentialHusband = familyPtr->husband;
                        if(potentialHusband->marriage == NULL && potentialHusband->data != 0){
                                cout << "Family " << familyPtr->data << " points to husband person " << potentialHusband->data << " but there is no back pointer." << endl;
                        }else if(potentialHusband->data != 0 && potentialHusband->marriage->data != familyPtr->data){
                                cout << "Family " << familyPtr->data << " points to husband person " << potentialHusband->data << " but there is no back pointer." << endl;
                        }
                }

                for(int childIndex = 0; childIndex < familyPtr->child.size(); childIndex++){
                        if(familyPtr->child[childIndex] != NULL){
                                Person *potentialChild = familyPtr->child[childIndex];
                                if(potentialChild->parents == NULL){
                                        cout << "Family " << familyPtr->data << " points to child  person " << potentialChild->data << " but there is no back pointer." << endl;
                                }
                        }
                }

        }

}

void relate(Person person[], Family family[], vector<string> command){
        int person1ID = stoi(command[1]);
        int person2ID = stoi(command[2]);

        if(person1ID == person2ID){
                cout << "You related the same person: shortest path between a person and himself is 0!" << endl;
        }else{
                relateHelper(person, person1ID, person2ID, false);

        } 
}

void relateHelper(Person person[], int person1, int person2, bool foundStatus){
        bool match = foundStatus;
        Person *personPtr, *husbandPtr, *wifePtr, *childPtr;
        Family *familyPtr, *parentsPtr, *mariagePtr;
        int personID = person1;

        if(person1 == person2){
                cout << "Person found." << endl;
                foundStatus = true;
        }

        personPtr = &person[personID];
        cout << "person" << personPtr->data << endl;

        if(personPtr->marriage != NULL){ // if person1's marriage connection to a family is not null
                familyPtr = personPtr->marriage; // set a family to the marriage pointer
                cout << "family" << familyPtr->data << endl;
                if(familyPtr->data != 0){ // check for valid pointer value                              

                        if(familyPtr->husband != NULL && familyPtr->husband->data != 0){
                                relateHelper(person, familyPtr->husband->data, person2, false);
                        }

                        if(familyPtr->wife != NULL && familyPtr->wife->data != 0){
                                relateHelper(person, familyPtr->wife->data, person2, false);
                        }

                        for(int i = 0; i < familyPtr->child.size(); i++){

                                if(familyPtr->child[i] != NULL && familyPtr->child[i]->data != 0){
                                       relateHelper(person, familyPtr->child[i]->data, person2, false);
                                }
                        }
                }
        }

        if(personPtr->parents != NULL){
                familyPtr = personPtr->parents;
                cout << "family" << familyPtr->data << endl;
                if(familyPtr->data != 0){ // check for valid pointer value

                        if(familyPtr->husband != NULL && familyPtr->husband->data != 0){
                                relateHelper(person, familyPtr->husband->data, person2, false);
                        }

                        if(familyPtr->wife != NULL && familyPtr->wife->data != 0){
                                relateHelper(person, familyPtr->wife->data, person2, false);
                        }

                        for(int i = 0; i < familyPtr->child.size(); i++){
                                if(familyPtr->child[i] != NULL && familyPtr->child[i]->data != 0){
                                        relateHelper(person, familyPtr->child[i]->data, person2, false);

                                }
                        }
                }
        }
}

vector<string> parse(string readLine){
        vector<string> returnVector; // vector for returning the string of broken down commands
        istringstream iss(readLine); // istringstream taken from http://www.cplusplus.com/reference/sstream/istringstream/istringstream/
        for(readLine; iss >> readLine; ) // for the string to be read, break the istringstream object by whitespace 
                returnVector.push_back(readLine); // push_back each word into vector 
        return returnVector;
}

int main(int argc, char **argv) {
        Person personArray[100]; // create an array of 100 people
        Family familyArray[100]; //  ''     ''    ''   100 families
        initilizeTree(personArray, familyArray);

        fstream myFile;
        myFile.open(argv[1]);
        string readLine;
        vector<string> commandVector;
        vector<string> personQueue;
        vector<string> familyQueue;

        while(getline(myFile, readLine)){
             commandVector = parse(readLine);
             string commandType = commandVector[0];
             if(commandType == "Person"){
                     buildPerson(personArray,familyArray,commandVector);
                     personQueue.push_back(commandVector[1]);
             }else if(commandType == "Family"){
                     buildFamily(personArray,familyArray,commandVector);
                     familyQueue.push_back(commandVector[1]);
             }else if(commandType == "Verify"){
                     verify(personArray, familyArray, personQueue, familyQueue);
                     personQueue.clear();
                     familyQueue.clear();
             }else if(commandType == "Relate"){
                     //relate(personArray,familyArray,commandVector);
             }
        }
        return 0;
}
