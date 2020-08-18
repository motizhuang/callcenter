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
}
std::vector<int> MyCallCenter::calls(int minute, const std::vector<int>& call_ids){
  for(const int& id : call_ids){
    Call info; 
    info.id             =id;                        
    info.recieved       =minute;            
    info.importance     =10;        
    info.difficulty     =1;        
    info.work_required  =3;  
    info.work_performed =0;
    mPool[info.difficulty].push(new Call(info)); 
  }
  if(too_hard()){//when realize the call is too hard, need to put back into pool 
    for(int i = 0; i<employeecount;i++){
      if(mEmployees[i].call!=nullptr)
      action[i]=mEmployees[i].call->id; 
    }
  }
  if(have_null()){
    for(int i = 0; i<employeecount;i++){
      if(mEmployees[i].call!=nullptr)
      action[i]=mEmployees[i].call->id; 
    }
  }
  if(have_important()){//important calls to take 
    for(int i = 0; i<employeecount;i++){
      if(mEmployees[i].call!=nullptr)
      action[i]=mEmployees[i].call->id; 
    }
  } //if(working())
  if(can_pick_up()){
    for(int i = 0; i<employeecount;i++){
      if(mEmployees[i].call!=nullptr)
      action[i]=mEmployees[i].call->id; 
    }
  }
  /*if(can_hang()){//should be after checks for -1
    for(int i = 0; i<employeecount;i++){
      if(mEmployees[i].call!=nullptr)
      action[i]=mEmployees[i].call->id; 
    }
  }*/
  can_hang();
  return action; 
}
bool MyCallCenter::too_hard(){
  int count=0; 
  for(int i = 0; i< employeecount; i++){
    Employee& employee = mEmployees[i]; 
    if(employee.call!=nullptr&&employee.call->difficulty>employee.skill){
      mPool[employee.call->difficulty].push(employee.call); 
      for(int j = 25; j>0; j--){
        if(!mPool[j].empty()&&j<=employee.skill){
          employee.call=mPool[j].top(); 
          mPool[j].pop();
        }
      }
      if(employee.call->difficulty>employee.skill){//so if there's none that person can take, need to evacuate that spot
        employee.call=nullptr;
        action[i]=0;
      }
      count++; 
    }
  }
  if(count!=0)
  return true; 
  return false; 
}
void MyCallCenter::can_hang(){
  int count=0; 
  for(int i = 0; i< employeecount; i++){
    Employee& employee = mEmployees[i]; 
    if(employee.call!=nullptr&&employee.call->work_required==employee.call->work_performed){
      action[i]=-1;
      std::cout<<employee.name<<" finished call. "<<'\n';
      count++; 
    }
  }
  /*if(count!=0)
  return true; 
  return false; */
}
bool MyCallCenter::have_null(){
  int count = 0; 
  for(int j = 25; j>=0; j--)
    for(int i = 0; i< employeecount; i++){
      Employee& employee = mEmployees[i]; 
      if(employee.call==nullptr){
        count++; 
        if(!mPool[j].empty()){
          if(j<=employee.skill){
            employee.call=mPool[j].top();
            mPool[j].pop();
          }
        }
      } 
    }
  if(count!=0)
  return true; 
  return false;
}
bool MyCallCenter::have_important(){
  //Call newcall; 
  for(int i = 1; i<26; i++)//wait if want difficult calls done first, just invert 26 first...
      for(int j =0; j<employeecount; j++ ){
        Employee& employee = mEmployees[j]; 
        if(employee.call!=nullptr&&!mPool[i].empty()&&mPool[i].top()->importance>employee.call->importance&&i<=employee.skill){//problem is if there's an employee down the line 
        //who is better can take that call instead
          //newcall = employee.call; 
          mPool[employee.call->difficulty].push(employee.call); //put on hold 
          employee.call=mPool[i].top(); //pick up 
          haveImportant.push_back(mPool[i].top());  
          mPool[i].pop();
        //alter this a little, should return true after looking through all the calls 
        }
      }
  if(!haveImportant.empty())
  return true; 
  return false; 
    //for(const auto& calls : mPool[i])//problem is you can't do this for a map, need a vector. 
  //there's a call more important on hold 
}
bool MyCallCenter::can_pick_up(){
  for(int i = 25; i>0; i--)
    for(int j =0; j<employeecount; j++ ){
      Employee& employee = mEmployees[j];
       
      if(employee.call!=nullptr&&(action[j]==0||action[j]==-1)&&!mPool[i].empty()){
        if(i<=employee.skill){
          employee.call=mPool[i].top();
          haveImportant.push_back(mPool[i].top());
          mPool[i].pop();
        }
      }
    }
  if(!haveImportant.empty())
  return true; 
  return false; 
}
void MyCallCenter::learn(int minute, const std::vector<Call>& calls){
  for(const auto& pool: calls){
    for(int j =0; j<employeecount; j++ ){
      Employee& employee = mEmployees[j];
      if(employee.call!=nullptr&&employee.call->id==pool.id){
        employee.call->recieved       =pool.recieved;            
        employee.call->importance     =pool.importance;        
        employee.call->difficulty     =pool.difficulty;        
        employee.call->work_required  =pool.work_required;  
        employee.call->work_performed =pool.work_performed;
      }
    }
    (void) minute; 
  } 
}

// MyCallCenter Member Functions
