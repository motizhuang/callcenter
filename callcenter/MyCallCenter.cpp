#include "MyCallCenter.h"
#include <vector> 
#include "Objects.h"
#include <string> 
#include <queue> 
#include <iostream>

CallCenter* CallCenter::create(std::vector<Employee> employees) {
  return new MyCallCenter(employees);
}
MyCallCenter::MyCallCenter(std::vector<Employee> employees){ 
  /*for(const auto& employee: employees) {
    Employee info;
    info.id       =employee.id;   
    info.name     =employee.name; 
    info.skill    =employee.skill;
    info.call     =employee.call; 
    mEmployees[employee.id]=info; 
  }*/
  mEmployees=employees; 
  employeecount = mEmployees.size();
  std::vector<int> starter(employeecount, 0);
  action = starter; 
  prevaction = starter; 
}
MyCallCenter::~MyCallCenter(){
  for(const Employee& employee: mEmployees) {
    delete employee.call;
  }
  for(int i = 1; i<26; i++)
  //while(const auto& pair: mPool[i]) {
    while(!mPool[i].empty()){
      delete mPool[i].top();
      mPool[i].pop();
  }
}
std::vector<int> MyCallCenter::calls(int minute, const std::vector<int>& call_ids){
  for(const int& id : call_ids){
    Call info; 
    info.id             =id;                        
    info.recieved       =minute;            
    info.importance     =10;        
    info.difficulty     =1;        
    info.work_required  =-1;  
    info.work_performed =0;
    mPool[info.difficulty].push(new Call(info)); 
  }
  can_hang(); 
  too_hard();
  can_pick_up();
  for(int i = 0; i<employeecount;i++){//keep track of work_performed
    if(mEmployees[i].call!=nullptr&&prevaction[i]==action[i]&&action[i]!=-1)
      mEmployees[i].call->work_performed++;
    /*if(mEmployees[i].call!=nullptr)
    action[i]=mEmployees[i].call->id; //if too hard or can pick up doesn't enter, then this is needed
    //but can't have it equal to id because -1 isn't an id. */
  }
  
  prevaction = action; 
  return action; 
}
void MyCallCenter::too_hard(){
  int count=0; 
  for(int i = 0; i< employeecount; i++){
    Employee& employee = mEmployees[i]; 
    if(employee.call!=nullptr&&employee.call->difficulty>employee.skill){
      for(int j = 0; j<employeecount;j++){
        Employee& another = mEmployees[j]; 
        if(another.skill>=employee.call->difficulty&&action[another.id]==0){
          another.call=employee.call; 
          action[i]=0; 
        }
      }
      if(action[i]!=0)
      mPool[employee.call->difficulty].push(employee.call);
      employee.call=nullptr;  
      action[i]=0; 
      count++;
    }
  }
  /*if(count!=0)
  return true; 
  return false; */
}
void MyCallCenter::can_hang(){
  int count=0; 
  for(int i = 0; i< employeecount; i++){
    Employee& employee = mEmployees[i]; 
    //if(employee.name=="Lucius")
    if(employee.call!=nullptr)
    std::cout<<employee.id<<"is working"<<employee.call->work_performed<<" on "<<employee.call->id<<" with "<<employee.call->work_required<<'\n';
    if(employee.call!=nullptr&&employee.call->work_required==employee.call->work_performed){
      action[i]=-1;
      //employee.call=nullptr; //screws stuff up. allows more work to be done when it shouldn't be. perhaps because some skill too low 
      std::cout<<employee.id<<" aaaaaaaaaaaaaaaaaaa finished call. "<<employee.call->id<<'\n';
      count++; 
    }
  }
}
bool MyCallCenter::have_important(){
  int count =0; 
  for(int i = 25; i>0; i--)
      for(int j =0; j<employeecount; j++ ){
        Employee& employee = mEmployees[j]; 
        if(employee.call!=nullptr&&!mPool[i].empty()&&mPool[i].top()->importance>employee.call->importance&&i<=employee.skill){
          mPool[employee.call->difficulty].push(employee.call);
          employee.call=mPool[i].top(); 
          mPool[i].pop(); 
          count++; 
        }
      }
  if(count!=0)
  return true; 
  return false; 
}
void MyCallCenter::can_pick_up(){
  int count = 0; 
  for(int j =0; j<employeecount; j++ ){
    Employee& employee = mEmployees[j];
    for(int i = 25; i>0; i--){
      if(prevaction[j]==-1){
        action[j]=0;
        delete employee.call; 
        employee.call=nullptr; 
      }
      if(employee.call==nullptr&&i<=employee.skill&&!mPool[i].empty()){
        employee.call=mPool[i].top();
        mPool[i].pop();
        count++;
        action[j]=employee.call->id;   
      } 
      if(/*employee.call!=nullptr&&*/action[j]==0&&!mPool[i].empty()){
        if(i<=employee.skill){
          employee.call=mPool[i].top();
          mPool[i].pop();
          count++; 
          action[j]=employee.call->id; 
        }
      }
    }
  }
}
void MyCallCenter::learn(int minute, const std::vector<Call>& calls){
  for(const auto& pool: calls){
    for(int j =0; j<employeecount; j++ ){
      Employee& employee = mEmployees[j];
      if(employee.call!=nullptr&&employee.call->id==pool.id){
        employee.call->recieved       =pool.recieved;            
        employee.call->importance     =pool.importance;        
        employee.call->difficulty     =pool.difficulty;
        if(employee.call->work_required!=pool.work_required){
          employee.call->work_performed=0; 
          //std::cout<<"does this work"<<'\n';
        }        
        employee.call->work_required  =pool.work_required;  
      }
    }
    (void) minute; 
  } 
}

// MyCallCenter Member Functions
