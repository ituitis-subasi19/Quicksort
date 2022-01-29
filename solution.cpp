//////////////////////////////////
/* Ayşe Zeynep Subaşı */
/* 150190046          */
//////////////////////////////////

// Compilation: g++ -std=c++11 solution.cpp
// ./a.out number_of_tweets randomized_or_deterministic input_file output_file
// ex: ./a.out 10000 randomized tweets_unsorted.csv out10k.csv

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

string upperString(const string& x){ // To compare two strings case-insenstive, I returned the uppercased version of the string in this function and made comparison between the return values
    string y = x;
    for(int i=0; i < x.length(); i++){
        y[i] = toupper(x[i]);
    }
    return y;
}

class Music{ // Created a class named Music with the values and functions all public to achieve them easily 
    public:
    string tweet_id;
    string tweet_datetime;
    string tweet_unixtime;
    string artist_name;
    string track_title;
    string country;
    Music(const string& id, const string& datetime, const string& unixtime, const string& artist, const string& title, const string& country_name) : tweet_id(id), tweet_datetime(datetime), tweet_unixtime(unixtime), artist_name(artist), track_title(title), country(country_name){}
    Music() : tweet_id(""), tweet_datetime(""), tweet_unixtime(""), artist_name(""), track_title(""), country(""){}
    bool operator>(const Music&);
};

bool Music::operator>(const Music& m){ // Written comparison function as wanted.
    string upperCountry1 = upperString(country); // First, I returned the uppercased values of the countries
    string upperCountry2 = upperString(m.country);
    if(upperCountry1 > upperCountry2) // Compared them alphabetically
        return true;
    else if(upperCountry1 == upperCountry2){ // If country names are the same, checked the artsit names
        string upperArtist1 = upperString(artist_name); // Returned the uppercase valus of the artistname
        string upperArtist2 = upperString(m.artist_name);
        if(upperArtist1 > upperArtist2) // Compared them alphabetically
            return true;
        else if(upperArtist1 == upperArtist2){ // If artist names are the same, checked the unix times
            if(tweet_unixtime > m.tweet_unixtime)
                return true;
            else
                return false;
        } else
            return false;
    } else
        return false;
}

void read_from_file(vector<Music>& musics, char* name, int num){
    fstream fin;
    fin.open(name, ios::in); // opened the input file to read
    string line, tweet_id, tweet_datetime, tweet_unixtime, artist_name, track_title, country;

    getline(fin, line); // get the first line of the file, since there are the names, didn't push to musics vector

    for(int i=0; i<num && getline(fin, line); i++){ // get all the lines till the end
        stringstream l(line);
        while(getline(l, tweet_id, '\t')){ // assign the values from the line to the variables by seperating them by tabs('\t')
            getline(l, tweet_datetime, '\t');
            getline(l, tweet_unixtime, '\t');
            getline(l, artist_name, '\t');
            getline(l, track_title, '\t');
            getline(l, country, '\t');
            Music music(tweet_id, tweet_datetime, tweet_unixtime, artist_name, track_title, country); // created a music object by using the variables
            musics.push_back(music); // and pushed it into the musics vector
        }
    }

    fin.close();
}

void write_to_file(vector<Music>& musics, char* name){
    fstream fout;
    fout.open(name, ios::out); // opened the input file to write

    fout << "tweet_id" << "\t" // wrote the headings into the file
         << "tweet_datetime" << "\t"
         << "tweet_unixtime" << "\t"
         << "artist_name" << "\t"
         << "track_title" << "\t"
         << "country" << endl;

    for(int i=0; i<musics.size(); i++){ // write the elements from the vector to the file
        fout << musics[i].tweet_id << "\t"
             << musics[i].tweet_datetime << "\t"
             << musics[i].tweet_unixtime << "\t"
             << musics[i].artist_name << "\t"
             << musics[i].track_title << "\t"
             << musics[i].country << endl;
    }
    fout.close();
}

int quick_sort_partition(vector<Music>& musics, int p, int q, string p_sel){
    Music x, temp; // created a music object named x to keep the value of the pivot

    if(p_sel == "randomized"){ // checked if the sort is randomized or not
        int a = rand() %(q-p) + p; // if randomized, randomly selected an index value
        x = musics[a]; // and kept the element in the index into x
        temp = musics[a]; // swapped the pivot with the first element to smoothly do the quicksort
        musics[a] = musics[p];
        musics[p] = temp;
    } else
        x = musics[p]; // if deterministic, selected the first element as pivot (first element into this sub)
    
    int i = p;
    for(int j = p+1; j < q; j++){ // compared all elements with the pivot, if the element is smaller than or equal to the pivot, seperate it from the bigger elemnts by taking it into the first part (where the smaller elements are)
        if(!(musics[j] > x)){
            i++;
            temp = musics[i];
            musics[i] = musics[j];
            musics[j] = temp;
        }
    }
    temp = musics[i]; // swapped the pivot (the first element into this sub) with the last smaller element (it divides the smaller and the bigger values) 
    musics[i] = musics[p];
    musics[p] = temp;
    return i; // returned the index of the pivot
}

void quick_sort(vector<Music>& musics, int p, int r, string p_sel){
    if(p < r){
        int q = quick_sort_partition(musics, p, r, p_sel); // first implemeted the partitoning for the array, then the parts of it seperated by the index returned from this function
        quick_sort(musics, p, q, p_sel);
        quick_sort(musics, q+1, r, p_sel);
    }
}

int main(int argc, char** argv){
    string p_sel = argv[2];

    if(argc != 5 || (p_sel != "randomized" && p_sel != "deterministic")) // if pivot selection is neither randomized nor deterministic or there are less inputs than necessary, returned 1
        return 1;

    srand(time(NULL));

    clock_t start, end;
    int num = stoi(argv[1]); // change the string number which is n to an integer

    vector<Music> musics;
    read_from_file(musics, argv[3], num); // read from the wanted file
    
    start = clock(); // start of the clock
    quick_sort(musics, 0, num, p_sel);
    end = clock(); // end of the clock
    
    write_to_file(musics, argv[4]); // wrote into wanted file
    
    cout << "Sorted in " << (end-start) * 1.0 /(CLOCKS_PER_SEC/1000) << " milliseconds by using " << p_sel << " pivot selection." << endl;
    return 0;
}