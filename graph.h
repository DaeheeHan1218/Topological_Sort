#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <list>
#include <queue>
#include <sstream>
#include <fstream>
#include <stdexcept>

using namespace std;

//edits to this are likely not needed
template <class T>
struct Vertex {
  T label; // unique int for a vertex
  vector<T> adj_list;
  int indegree; //number of nodes pointing in
  int top_num; //topological sorting number
  Vertex(T l) : label(l) {top_num = 0;}
  Vertex() { label = T(); indegree = 0; top_num = 0; }
};

template <class T>
class Graph {
private:
  unordered_map<T, Vertex<T>> node_set;
  int num_vertices;
  
public:
  Graph() {};
  ~Graph() {};

  void buildGraph(istream &input){
	  string line;
	  bool first = true;
	  T val;
	  num_vertices = 0;
	  while(getline(input, line)){
		  stringstream ss(line);
		  first = true;
		  Vertex<T> ins;
		  ins.indegree = 0;
		  while(ss >> val){
			  if(first){
				  ins.label = val;
				  first = false;
			  }
			  else{
				  ins.adj_list.push_back(val);
			  }
		  }
		  ins.indegree = 0;
		  node_set[ins.label] = ins;
		  num_vertices++;
	  }
  }

  // display the graph into o
  void displayGraph(ostream& o){
	  for(auto x:node_set){
		  o << x.first << " : ";
		  for(int i = 0; i < x.second.adj_list.size(); i++){
			  o << x.second.adj_list[i] << " ";
		  }
		  o << "\n";
	  }
  }

  //return the vertex at label, else throw any exception
  Vertex<T> at(T label){
	  Vertex<T> v = node_set[label];
	  if(v.label != label){
		  node_set.erase(label);
		  throw std::invalid_argument( "non-existing label" );
	  }
	  else{
		  return v;
	  }
  }

  //return the graph size (number of verticies)
  int size(){
	  return num_vertices;
  }

  // topological sort
  // return true if successful, false on failure (cycle)
  bool topological_sort(){
	  queue<Vertex<T>> q;
	  int cnt = 0;
	  for(auto x:node_set){
		  if(x.second.indegree == 0){
			  q.push(x.second);
		  }
	  }
	  while(!q.empty()){
		  Vertex<T> v = q.front();
		  q.pop();
		  v.top_num = ++cnt;
		  node_set[v.label].top_num = cnt;
		  for(int i = 0; i < v.adj_list.size(); i++){
			  if(--node_set[v.adj_list[i]].indegree == 0){
				  q.push(node_set[v.adj_list[i]]);
			  }
		  }
	  }
	  if(cnt != num_vertices){
		  return false;
	  }
	  return true;
  }

  // find indegree
  void compute_indegree(){
	  for(auto x:node_set){
		  for(int i = 0; i < x.second.adj_list.size(); i++){
			  node_set[x.second.adj_list[i]].indegree++;
		  }
	  }
  }


  // print topological sort into o
  //  if addNewline is true insert newline into stream
  void print_top_sort(ostream& o, bool addNewline=true)
  { 
    vector<T> list;
	T temp;
	for(int i = 0; i < num_vertices; i++){
		list.push_back(temp);
	}
	for(auto x:node_set){
		list[x.second.top_num - 1] = x.first;
	}
	for(int i = 0; i < num_vertices; i++){
		o << list[i] << " ";
	}
    if(addNewline){o << '\n';};
  };
};

#endif
