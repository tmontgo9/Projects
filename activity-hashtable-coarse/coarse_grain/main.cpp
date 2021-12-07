#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>
#include <mutex>
#include <thread>
#include <iostream>

#include "Dictionary.cpp"
#include "MyHashtable.cpp"



std::vector<std::vector<std::string>> tokenizeLyrics(const std::vector<std::string> files) {
  std::vector<std::vector<std::string>> ret;

  for(auto filename : files) {

    std::vector<std::string> my_vect;
    std::ifstream in (filename);

    std::string line;

    //For each line
    while (getline(in, line, '\n')) {
      if (line[0] == '[')
        continue;

      std::stringstream ssline (line);
      //For all words
      while (ssline) {
        std::string word;
        ssline >> word;
        if (ssline) {
          while (word.length() > 0
                 && std::ispunct(word[word.length() - 1])) {
            word.pop_back();
          }
          my_vect.push_back(word);
        }
      }
    }
    ret.push_back(my_vect);
  }
  return ret;
}

void function(std::vector<std::string>& file, std::mutex& mut, Dictionary<std::string, int>& dict) {
  for(auto& w: file){
  mut.lock();
    int count = dict.get(w);
    ++count;
    dict.set(w, count);
    mut.unlock();
  }
}

int main(int argc, char **argv)
{
  if (argc < 4) {
    std::cerr<<"usage: ./main <sources> <testword> <threshold>"<<std::endl;
    return -1;
  }

  std::string source = argv[1];
  std::string testWord = argv[2];
  int32_t thresholdCount = std::stoi(argv[3]);

  std::vector<std::string> files;
  std::ifstream in (source);
  std::string line;
  while (getline(in, line, '\n')) {
    files.push_back(line);
  }

  auto wordmap = tokenizeLyrics(files);

  MyHashtable<std::string, int> ht;
   Dictionary<std::string, int>& dict = ht;
   std::mutex mutex;


   auto start =std::chrono::steady_clock::now();

   std::vector<std::thread> myThreads;
  
    for (auto & filecontent: wordmap) {
      std::thread myThread2 (function, std::ref(filecontent), std::ref(mutex), std::ref(dict) );
    myThreads.push_back(std::move(myThread2));
    }
    for(auto & t : myThreads){
      if(t.joinable())
    t.join();
      else
    std::cout<<"Does not join";
    }

  auto stop = std::chrono::steady_clock::now();
  std::chrono::duration<double> time_elapsed = stop-start;
  std::cerr << time_elapsed.count()<<"\n";

  // Check Hash Table Values
  /* (you can uncomment, but this must be commented out for tests)
  for (auto it : dict) {
    if (it.second > thresholdCount)
      std::cout << it.first << " " << it.second << std::endl;
  }
  */

  // Do not touch this, need for test cases
  std::cout << ht.get(testWord) << std::endl;
  

  return 0;
}
