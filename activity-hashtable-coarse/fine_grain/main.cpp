#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>
#include "Dictionary.cpp"
#include "MyHashtable.cpp"
#include<thread>
#include<mutex>
#include<iostream>

std::vector<std::vector<std::string>> tokenizeLyrics(const std::vector<std::string> files) {
  std::vector<std::vector<std::string>> ret;

  for(auto filename : files) {
    std::vector<std::string> my_vect;
    std::ifstream in (filename);

    std::string line;

    while (getline(in, line, '\n')) {
      if (line[0] == '[')
        continue;

      std::stringstream ssline (line);
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

void count(MyHashtable<std::string, int> &dict, std::vector<std::string> filecontent, std::mutex& mut){
  
  for (int i=0; i<filecontent.size(); i++) {
      dict.update(filecontent[i]);
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

  std::vector<std::thread> thread;
  std::mutex mut;

  auto start = std::chrono::steady_clock::now();

  for(auto & filecontent:wordmap){
    thread.push_back(std::thread(count,std::ref(ht),std::ref(filecontent),std::ref(mut)));
  }
  for(auto & t : thread){
    t.join();
  }

  auto stop = std::chrono::steady_clock::now();
  std::chrono::duration<double> time_elapsed = stop-start;

  for (auto it : dict) {
    if (it.second > thresholdCount)
      std::cout << it.first << " " << it.second << std::endl;
  }
  

  // Do not touch this, need for test cases
  std::cout << ht.get(testWord) << std::endl;
  std::cerr << time_elapsed.count()<<"\n";

  return 0;
}
