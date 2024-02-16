// Gabriel Cadieux
// Last Edited 2/5/23
// Reads files of input data that consists of music artists, some albums, some songs, and related information
// It then restructures all the data read in, and prints it in a specific format

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <algorithm>
#include <iomanip>
using namespace std;

/////////////////////// BEGIN STRUCTS //////////////////////////////
struct Song {
	string title;
	string time;
	int tracknumber;
};

struct Album {
	map <int, Song > songs;
	string name;
	int time;
	int mins;
	int secs;
	int nsongs;
};

struct Artist {
	map <string, Album > albums;
	string name;
	int time; 
	int nsongs;
};
//////////////////////// END STRUCTS //////////////////////////////


//////////////////////// BEGIN MAIN ///////////////////////////////
int main(int argc, char *argv[]) {
	
	map <string, Artist> artists;
	string test;
	ifstream inFile;				// Uses file streaming to to open needed file to be read in
	inFile.open(argv[1]);

	// Note: I structured my program a little weird, as I typically immediately made operations after I read a "word"
	// in, I realized much later on I could have read each word and stored everything I needed at the start, then
	// performed operations, but since I was following the recommended steps, it was more of an "as I go" kind of thing

	while(getline (inFile, test)) { // Begin of while loop (main program)
		
		stringstream ss(test);
		string word;

		// First word: Title.
		ss >> word;
		string songname = word; // Store first word as songname
		
		// Second word: Time.
		ss >> word;
		string songtime = word; // Store the song time portion to be printed later
		int totalseconds = 0;
		int minutes = 0;
		int seconds = 0;
		
		// Both of the following if statements are essentially the same, the use a weird method in order to calculate
		// the total seconds of the given songtime, but it works for me

		if (songtime.size() == 5) { // Here, if the songtime length was in format XX:XX perform this calculation
			
			int minute1 = songtime[0];
			minute1 = (minute1 - 48) * 10;
			int minute2 = songtime[1];
			minute2 = (minute2 - 48);
			minutes = minute1 + minute2;

			int second1 = songtime[3];
			second1 = (second1 - 48) * 10;
			int second2 = songtime[4];
			second2 = (second2 - 48);
			seconds = second1 + second2;

			totalseconds = seconds + (minutes * 60);
			

		}
		else if (songtime.size() == 4) { // Here, if the songtime length was in format X:XX perform this calculation
			
			minutes = songtime[0];
			minutes = minutes - 48;
			
			int second1 = songtime[2];
			second1 = (second1 - 48) * 10;
			
			int second2 = songtime[3];
			second2 = second2 - 48;
			
			seconds = second1 + second2;
			totalseconds = seconds + (minutes * 60);
			

		}
		else {
			cout << "Messed up!\n";
		}

		// Third Word : Artist.
		ss >> word;
		string artistname = word;	// Here I save the artistname so I can access it later
		for (int i = 0; i < artistname.length(); i++) { // Here I remove the underscores in the artistname
			if (artistname[i] == '_') {
				artistname[i] = ' ';
			}
		} // End of For loop
	
		if (artists.count(artistname) != 0) {	// Artist found, already in map, update number of songs and time
			artists[artistname].nsongs++;
			artists[artistname].time = artists[artistname].time + totalseconds;
		}
		else {							// Artist Not Found, add them to the map, initialize nsongs, time, and name
			struct Artist person;
			person.name = artistname;
			person.time = totalseconds;
			person.nsongs = 1;
			artists[artistname] = person;
		} // End of if-else
		
		//Fourth word : Album.
		ss >> word;
		string albumname = word;	// Store the albumn name
		for (int i = 0; i < albumname.length(); i++) { // Remove the underscores just like artistname
			if (albumname[i] == '_') {
				albumname[i] = ' ';
			}
		}
		
		// Big note: Here I realized I am a bit silly and did not need to save and calculate the min and secs to
		// be printed everytime I added the album, and that I could have just printed time / 60 at the end when I 
		// went to print, however, I wrote this bit days ago and would rather keep it the way it is than change it
		// However, I will acknowledge this was a sub-optimal way to solve this problem

		if (artists[artistname].albums.count(albumname) != 0) {	// Album found, works much like artist method
			artists[artistname].albums[albumname].nsongs++;
			artists[artistname].albums[albumname].time += totalseconds;
			int min =  artists[artistname].albums[albumname].time / 60;
			int sec =  artists[artistname].albums[albumname].time % 60;
			artists[artistname].albums[albumname].mins = min; 
			artists[artistname].albums[albumname].secs = sec; 
		}
		else {							// Album Not Found, works much like artist method
			struct Album newalbum;
			newalbum.name = albumname;
			newalbum.time = totalseconds;
			newalbum.nsongs = 1;
			artists[artistname].albums[albumname] = newalbum;
			artists[artistname].albums[albumname].mins = artists[artistname].albums[albumname].time / 60;
			artists[artistname].albums[albumname].secs = artists[artistname].albums[albumname].time % 60;
		} // End of if-else
		
		// Fifth Word : Genre
		ss >> word;
		string genre = word; // Genre for some reason is unnecessary to print, but save it anyways
		
		// Sixth Word : Track Number
		ss >> word;
		int tracknum = 0;
		if (word.length() == 2) {
			int digit1 = word[0];
			digit1 = (digit1 - 48) * 10;	// Here we are converting the track number provided into
			int digit2 = word[1];			// a usable int, a little convoluted but it works
			digit2 = digit2 - 48;
			tracknum = digit1 + digit2;
		}
		else if (word.length() == 1) {
			tracknum = word[0];
			tracknum = tracknum - 48;
		}

		for (int i = 0; i < songname.length(); i++) {	// Remove underscores from the name of the song
			if (songname[i] == '_') {
				songname[i] = ' ';
			}
		}
		struct Song newsong;
		newsong.title = songname;		// Initialize a new song under the current artist and album
		newsong.time = songtime;		// note we do not need to check if the song is already there
		newsong.tracknumber = tracknum;	// because an artist should not have songs with the same track number
		artists[artistname].albums[albumname].songs[tracknum] = newsong;	// in the same salbum
	

	} // End of while loop
	

	// Method for printing out all stored information
	
	for (map<string, Artist>::iterator it= artists.begin(); it != artists.end(); ++it) {
			
		cout << it->first << ": " << it->second.nsongs << ", " 
			 << it->second.time / 60 << ":";							// Iterate through artists and print correctly
		cout << setfill('0') << setw(2) << it->second.time % 60 << "\n";

		// Before we loop again, print the albums associated with current artist and the albums associated song
		for(map<string, Album>::iterator it2= it->second.albums.begin(); it2 != it->second.albums.end(); ++it2) {

			cout << "        " << it2->first << ": " << it2->second.nsongs << ", " << it2->second.mins << ":"; 
			cout << setfill('0') << setw(2) << it2->second.secs << "\n";

			for (map<int, Song>::iterator it3= it2->second.songs.begin(); it3 != it2->second.songs.end(); ++it3) {
				
				cout<< "                " << it3->first << ". " << it3->second.title << ": " << it3->second.time << "\n";
			}
		}
	}



}
