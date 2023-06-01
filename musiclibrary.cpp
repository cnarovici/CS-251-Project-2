/// TODO Update File Header
///
///Name: Cosmin Narovici
///
///General Overview: Takes in a file with an album which may contain an artist and tracks and then stores said strings
///to then get data on them and be able to parse through the albums. 
///
/// Assignment details and provided code are created and
/// owned by Adam T Koehler, PhD - Copyright 2023.
/// University of Illinois Chicago - CS 251 Spring 2023

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <fstream>
#include <cctype>
#include <iterator>

using namespace std;

#include "helpers.cpp"

//Struct use to store all artist and track names in sets for each map key.
struct artAndTracks
{
    set <string> artist;
    set <string> tracks;
};
//Prints the names of the artists when stats -d is input
//Takes in a set of strings which will then be printed.
void printArtists(set<string> &artists) {
    for (auto& name : artists) {
        cout << " " << name << endl;
    }
}
//Prints the names of the tracks when stats -d is input
//Takes in a set of strings which will then be printed.
void printSongs(set<string> &songs) {
    for (auto& song : songs) {
        cout << "   " << song << endl;
    }
}
//Used to export artist names when export is called.
//Takes in the set of artists and an ofstream to export the artists in the set.
void outArtists(set<string> &artists, ofstream &oFS) {
    for (auto& name : artists) {
        oFS << name << endl;
    }
}
//Used to export track names when export is called.
//Takes in the set of songs and an ofstream to export said songs.
void outSongs(set<string> &songs, ofstream &oFS) {
    for (auto& song : songs) {
        oFS << song << endl;
    }
}
//Based on splitFirstWord but it cuts down the string instead permanently.
//Takes in 2 strings and continues to cut down the string until it is empty/
void firstWordAndRest(string &remains, string &first)
{
    // first word or possibly the only word
    first = remains.substr(0,remains.find(" "));
 
    // skip all whitespace and get everything after the first word
    if (remains.size() != first.size())
    {
        remains = trimWhiteSpace(remains.substr(remains.find(" ")));
    }
    else
    {
        remains = "";
    }

}
//Searches for the terms that are input with the search command.
//Takes in a string to not directly modify remains, string to store the first string 
//returned by firstWordAndRest, the set searchTerm which is the set of albums being searched for. 
//insersectionTerms where the terms that have the + modifier go and difference terms where terms with - are stored.
void sortSearchTerms(string &tempRemains, string &tempFirst, set<string> &searchTerm, set<string> &intersectionTerms, set<string> &differenceTerms) {
    // firstWordAndRest(tempRemains, tempFirst);
    while (tempRemains != "") {
        if (tempFirst.substr(0,1) == "+") {
            intersectionTerms.insert(tempFirst);
            firstWordAndRest(tempRemains, tempFirst);

        } else if (tempFirst.substr(0,1) == "-") {
            differenceTerms.insert(tempFirst);
            firstWordAndRest(tempRemains, tempFirst);

        } else {
            searchTerm.insert(tempFirst);
            firstWordAndRest(tempRemains, tempFirst);
            
        }
    }
}
//Stores the names of albums that were searched for.
//Takes in a set to store intersecting terms, the difference terms, the terms being searched for, 
//the terms being stored, the map with all albums, and a temp string for other uses.
void storeIntersecting(set<string> &intersectionTerms, set<string> &differenceTerms, set<string> &searchTerm, set<string> &searchResult, map<string, artAndTracks> &albums, string &temp) {
    for (auto& elem : albums) {
        temp = elem.first;
        tolower(temp);
        for (auto& elem2 : searchTerm) {
            if (elem2 == temp) {
                searchResult.insert(elem.first);
            }
        }
    }
}
//Prints the album which was searched for.
//Takes in the set that stored the results of matching albums.
void printSearch(set<string> &searchResult) {
    cout << "Your search results exist in the following albums: " << endl;
    for (auto& elem : searchResult) {
        cout << elem << endl;
    }
    cout << endl;
}

int main()
{
    string userEntry;
    string command, remains;

    // TODO: Declare any necessary variables for the music libary
    map <string, artAndTracks> albums;
    set <string> uniqueArtists;
    set <string> albumsSet;
    set <string> searchTerm;
    set <string> searchResult;
    set <string> intersectionTerms;
    set <string> differenceTerms;
    set <string> intersectionResult;
    set <string> differenceResult;
    ifstream inFS;
    ofstream oFS;
    string albumName;
    string temp;
    string tempRemains;
    string tempFirst;
    bool loadFlag = true;
    int totalSongs;
    // Display welcome message once per program execution
    cout << "Welcome to the Music Library App" << endl;
    cout << "--------------------------------" << endl;

    // Stay in menu until exit command
    do
    {
        cout << endl;
        cout << "Enter a command (help for help): " << endl;
        getline(cin, userEntry);
        cout << endl;

        // Split the user entry into two pieces
        splitFirstWord(userEntry, command, remains);
        tolower(command);

        // take an action, one per iteration, based on the command
        if (command == "help")
        {
            helpCommand();
        }
        else if (command == "clear")
        {
            // TODO
            uniqueArtists.clear();
            totalSongs = 0;
            albums.clear();
        }
        else if (command == "export")
        {
            // Opens file to be read.
            oFS.open(remains);
            //Sets default file if export is typed in with no extra arg.
            if (remains == "") {
                oFS.open("musicdatabase.txt");
            }
            //The exporting is done through calling the out functions.
            for (auto& elem : albums) {
                oFS << elem.first << endl;
                outArtists(elem.second.artist, oFS);
                outSongs(elem.second.tracks, oFS);
                oFS << elem.first << endl;
            }
        }
        else if (command == "load")
        {
            // Opens file to be read.
            inFS.open(remains);
            //Sets default file if load is typed in with no extra args.
            if (remains == "") {
                remains = "musicdatabase.txt";
                inFS.open(remains);
            }
            //Checks for valid file if arg is provided after load.
            if (!(inFS.is_open())) {
                cout << "Error: Could not open music library file - " << remains << endl;
                loadFlag = false;
            } else {
                getline(inFS, albumName);
                artAndTracks aT;
                albums.emplace(albumName, aT);
                albumsSet.insert(albumName);
            }
            //Handles all sorting of the data from the text file into a map that contains structs with sets of the artists and songs in the album.
            while (!(inFS.eof()) && loadFlag) {
                while (getline(inFS, temp))
                {
                    
                    if (albumName != temp) {
                        if (!isalpha(temp[0])) {
                            albums[albumName].tracks.insert(temp);
                        } else {
                            albums[albumName].artist.insert(temp);
                            uniqueArtists.insert(temp);
                        }
                    } else {
                        break;
                    }
                }
                getline(inFS, albumName);
                artAndTracks aT;
                albums.emplace(albumName, aT);
                albumsSet.insert(albumName);
            }
            inFS.close();
            albums.erase("");
        }
        else if (command == "stats")
        {
            // Tracks the total songs across the albums
            for (auto& elem : albums) {
                totalSongs += elem.second.tracks.size();
            }

            cout << "Overall Music Library Stats\n===========================" << endl;
            cout << "Total Records: " << albums.size() << endl;
            cout << "Total Unique Artists: " << uniqueArtists.size() << endl;
            cout << "Total Songs: " << totalSongs << "\n" << endl;
            //Checks for the extra arg -d and then prints library if it is provided.
            if (remains == "-d") {
                cout << "Your Current Music Library Includes\n===================================" << endl;
                for (auto& elem : albums) {
                    cout << elem.first << endl;
                    printArtists(elem.second.artist);
                    printSongs(elem.second.tracks);
                }
                cout << endl;
            }
        }          
        else if (command == "search")
        {
            // Checks for valid arg.
            if (remains == "") {
                cout << "Error: Search terms cannot be empty.\nNo results found.\n" << endl;
            } else if (remains.substr(0, 5) == "album") {
                tempRemains = remains;
                sortSearchTerms(tempRemains, tempFirst, searchTerm, intersectionTerms, differenceTerms);

                storeIntersecting(intersectionTerms, differenceTerms, searchTerm, searchResult, albums, temp);

                printSearch(searchResult);
            }


        }

    }while(command != "exit");    

    cout << "Thank you for using the Music Library App" << endl;
    return 0;
}
