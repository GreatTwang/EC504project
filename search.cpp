#include<iostream>
#include <stdio.h>
#include<vector>
#include<map>
#include<unordered_map>
#include<string>
#include<fstream>
#include<sstream>
#include<queue>
#include <fstream>
using namespace std;


struct word_position
{
  string file_name;
  string sentence;
  int line;
  int index;
};


struct compare{
    bool operator()(pair<string, int> & a, pair<string, int> & b){
        return a.second < b.second;
    }
};

class InvertedIndex
{
  // data structure 1
  unordered_map<string,vector<word_position> > dictionary;

  // data structure 2
  unordered_map<string,unordered_map<string,int> > dictionary2;

  // data structure 3
  map<string,vector<word_position> > dictionary3;

  // data structure 4
  map<string,map<string,int> > dictionary4;


  unordered_map<string,int> similarity;


  vector<string> filelist;

  public:
    void addfile(string filename);
    void addfile2(string filename);
    void show_files();
    void search();
    void search2();
    void top1();
    void top2();
};

void InvertedIndex::addfile(string filename)
{
  ifstream fp;
  fp.open(filename,ios::in);

  if(!fp)
  {
    cout<<"File Not Found\n";
    return;
  }

  filelist.push_back(filename);

  string line,word;
  int line_number=0,word_number=0;

  while(getline(fp,line))
  {
    //strtk::token_grid grid(line,line.size(),",");
    //cout<<line;

    line_number++;
    //if (line_number>10) break;
    word_number = 0;
    stringstream s(line);
    while(s>>word)
    {
      word_number++;
      word_position obj;
      obj.file_name = filename;
      obj.line = line_number;
      obj.index = word_number;
      obj.sentence = line;
      dictionary[word].push_back(obj);
    }
  }
  fp.close();
}

void InvertedIndex::addfile2(string filename)
{
  ifstream fp;
  fp.open(filename,ios::in);

  if(!fp)
  {
    cout<<"File Not Found\n";
    return;
  }

  filelist.push_back(filename);

  string line,word;
  int line_number=0,word_number=0;
  while(getline(fp,line,','))
  {
    line_number++;
    word_number = 0;
    stringstream s(line);
    while(s>>word)
    {
      dictionary2[word][line]++;
    }
  }
  fp.close();
}

void InvertedIndex::show_files()
{
  int size = (int)filelist.size();
  for(int i=0;i<size;i++) cout<<i+1<<": "<<filelist[i]<<endl;

  if(!size) cout<<"No files added\n";
}

void InvertedIndex::search()
{
  string words;
  cout<<"Enter Word: \n";
  cin.ignore();
  getline(cin, words);
  cout<<words<<endl;
  stringstream s(words);
  string word;
  while(s>>word)
  {
    cout<<word<<endl;
    if(dictionary.find(word)==dictionary.end()){
      cout<<"No instance exist\n";
      continue;
    }
    int size = (int)dictionary[word].size();
    for(int counter = 0;counter < size;counter++)
    {
      word_position obj = dictionary[word][counter];
      if(similarity.find(obj.sentence)==similarity.end())
      {
        similarity[obj.sentence]=1;
      }
      else
      {
        similarity[obj.sentence]++;
      }
    }
  }
  //cout<<similarity.size()<<endl;
  // for (auto it = similarity.begin(); it != similarity.end(); ++it) {
  //   cout<<it->first<<": "<<it->second<<endl;
  // }
}
void InvertedIndex::search2()
{
  string words;
  cout<<"Enter Word: \n";
  cin.ignore();
  getline(cin, words);
  cout<<words<<endl;
  stringstream s(words);
  string word;
  while(s>>word)
  {
    cout<<word<<endl;
    if(dictionary2.find(word)==dictionary2.end()){
      cout<<"No instance exist\n";
      continue;
    }
    int size = (int)dictionary2[word].size();
    for(auto i: dictionary2[word]){
          if(similarity.find(i.first)==similarity.end()){
            similarity[i.first]= 1;
          }
          else{ similarity[i.first]++;}
    }
  }
  //cout<<similarity.size()<<endl;
  // for (auto it = similarity.begin(); it != similarity.end(); ++it) {
  //   cout<<it->first<<": "<<it->second<<endl;
  // }
}
  
void InvertedIndex::top1(){
  priority_queue<pair<string, int>,vector<pair<string, int> >,compare> pq;
  int k= similarity.size()<10?similarity.size():10;
  // NlogK
  for (auto t: similarity) {
    pair<string,int> temp=make_pair(t.first, t.second);
    pq.push(temp);
    if (pq.size() < k) {
      pq.push(temp);
    }
    else if (pq.top().second < t.second) {
        pq.pop();
        pq.push(temp);
    }
  }

  //for(auto t : similarity) pq.push(make_pair(t.first, t.second));
  vector<string> res;
  for(int i = 0 ; i < k ; i++){
    res.push_back(pq.top().first);
    pq.pop();
  }

  //
  for(int i=0;i<res.size();i++){
    cout<<res[i]<<endl;
  }

  // clear
  similarity.clear();
}

void InvertedIndex::top2(){
  int length=10;
  vector<string> bucket[length];
  int k= similarity.size()<10?similarity.size():10;
  // N
  for (auto t: similarity) {
    bucket[t.second].push_back(t.first);
    }

  for(int i=length-1;i>=0;i--){
    if (bucket[i].size()>0){
      for (auto s:bucket[i]){
        if (k<=0) break;
        cout<<s<<endl;
        k--;
      }
    if (k<=0) break;
    }
  }

  // clear
  similarity.clear();
}

int main(int argc, char*argv[])
{

  InvertedIndex Data;
  for(int i = 1 ; i< argc ; i++)
  {
    Data.addfile(argv[i]);
  }

  int choice = 0;
  do
  {
    cout<<"1: See files\n2: Add File\n3: Query Word\n4: Exit\n";
    cin>>choice;
    switch(choice)
    {
      case 1: Data.show_files(); break;
      case 2:
      {
        cout<<"Enter File Name: ";
        string name;
        cin>>name;
        double startt1 = clock(); 
        Data.addfile(name);
        
        double endt1 = clock();
        double TT1 = (double)(endt1-startt1)/CLOCKS_PER_SEC;
        printf("FINISHED --- TOTAL CPU TIME %f SECS \n",(float)TT1);
        double startt11 = clock(); 
        Data.addfile2(name);
        double endt11 = clock();
        double TT11 = (double)(endt11-startt11)/CLOCKS_PER_SEC;
        printf("FINISHED --- TOTAL CPU TIME %f SECS \n",(float)TT11);
        break;
      }

      case 3:
      {
        double startt = clock(); 
        Data.search();
        Data.top1();
        double endt = clock();
        double TT2 = (double)(endt-startt)/CLOCKS_PER_SEC;
        printf("FINISHED --- TOTAL CPU TIME %f SECS \n",(float)TT2);
        cout<<"next round"<<endl;
        double startt3 = clock(); 
        Data.search2();
        Data.top1();
        double endt3 = clock();
        double TT3 = (double)(endt3-startt3)/CLOCKS_PER_SEC;
        printf("FINISHED --- TOTAL CPU TIME %f SECS \n",(float)TT3);

        cout<<"next round"<<endl;
        double startt4 = clock(); 
        Data.search2();
        Data.top2();
        double endt4 = clock();
        double TT4 = (double)(endt4-startt4)/CLOCKS_PER_SEC;
        printf("FINISHED --- TOTAL CPU TIME %f SECS \n",(float)TT4);
        break;
      }

      case 4: break;

      default : continue;
    }
  }while(choice!=4);

  return 0;
}