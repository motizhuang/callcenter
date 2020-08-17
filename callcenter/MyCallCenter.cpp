#include "MyCallCenter.h"
#include <vector> 
#include "Objects.h"
#include <string> 
#include <queue> 

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
  /*for(const Employee& person : mEmployees){
    action[person.id]=0; 
  }*/
  
  employeecount = mEmployees.size();
  std::vector<int> starter(employeecount, 0);
  action = starter; 
}
std::vector<int> MyCallCenter::calls(int minute, const std::vector<int>& call_ids){
  //thisminute = minute; 
  //x = new std::priority_queue<Call,std::vector<Call> ,Compare>[26];//to use indices as the levels of difficulty? 
  //unknown. low difficulties and high importance
  for(const int& id : call_ids){
    Call info; 
    info.id             =id;                        
    info.recieved       =minute;            
    info.importance     =10;        
    info.difficulty     =1;        
    info.work_required  =0;  
    info.work_performed =0;
    mPool[info.difficulty].push(new Call(info)); 
  }
  if(have_null()){
    for(int i = 0; i<employeecount;i++){
      if(mEmployees[i].call!=nullptr)
      action[i]=mEmployees[i].call->id; 
    }
  }else{
  if(have_important()){//important calls to take 
    for(int i = 0; i<employeecount;i++){
      action[i]=mEmployees[i].call->id; 
    }
  } //if(working())
  if(can_pick_up()){
    for(int i = 0; i<employeecount;i++){
      action[i]=mEmployees[i].call->id; 
    }
  }
  if(can_hang()){//should be after checks for -1
    for(int i = 0; i<employeecount;i++){
      action[i]=mEmployees[i].call->id; 
    }
  }
  }
  return action; 
}
bool MyCallCenter::can_hang(){
  int count=0; 
  for(int i = 0; i< employeecount; i++){
    Employee& employee = mEmployees[i]; 
  if(employee.call->work_required==employee.call->work_performed){
  action[i]=-1;
  count++; 
  }
  }
  if(count!=0)
  return true; 
  return false; 
}
bool MyCallCenter::have_null(){
  int count = 0; 
  for(int j = 1; j<26; j++)
  for(int i = 0; i< employeecount; i++){
    Employee& employee = mEmployees[i]; 
    if(employee.call==nullptr)
    count++; 
    if(!mPool[j].empty()){
      if(j<=employee.skill){
        employee.call=mPool[j].top();
        haveImportant.push_back(mPool[j].top());
        mPool[j].pop();
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
        if(!mPool[i].empty()&&mPool[i].top()->importance>employee.call->importance&&i<=employee.skill){//problem is if there's an employee down the line 
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
  for(int i = 0; i<26; i++)
    for(int j =0; j<employeecount; j++ ){
      Employee& employee = mEmployees[j]; 
      if((action[employee.id]==0||action[employee.id]==-1)&&!mPool[i].empty()){
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
    Call* info = nullptr; 
    info->id             =pool.id;                        
    info->recieved       =pool.recieved;            
    info->importance     =pool.importance;        
    info->difficulty     =pool.difficulty;        
    info->work_required  =pool.work_required;  
    info->work_performed =pool.work_performed;
    mPool[info->difficulty].push(info);  
  }
  /*START OF MINUTE
1) Learn new call IDs
2) Put calls on hold
3) Take calls out of hold
4) Learn answered call info
END OF MINUTE*/
//*when a calls in vector change, no work can be done. if it was in the same place the last minute, this minute it does work. 
  (void) minute; 
}

// MyCallCenter Member Functions
