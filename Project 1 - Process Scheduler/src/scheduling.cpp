#include <string>
#include <fstream>
#include <iostream>
#include <queue>
#include <list>
#include <sstream>

#include "scheduling.h"

using namespace std;

pqueue_arrival read_workload(string filename)
{
  //first read the file, make the processes and store them into the
  ifstream initfile;
  initfile.open(filename);
  string str;
  int num1, num2;
  pqueue_arrival workload;
  while (initfile >> num1 >> num2)
  {
    Process s;
    s.arrival = num1;
    s.duration = num2;
    s.first_run=-1;
    workload.push(s);
  }

  // stringstream ss;

  //   if(initfile.is_open()){
  //     while(getline(initfile,str)){
  //       ss<<str;
  //       ss>>num1>>num2;
  //       Process s;
  //       s.arrival=num1;
  //       s.duration=num2;
  //       workload.push(s);

  //     }

  //   }

  return workload;
}

void show_workload(pqueue_arrival workload)
{
  pqueue_arrival xs = workload;
  cout << "Workload:" << endl;
  while (!xs.empty())
  {
    Process p = xs.top();
    cout << '\t' << p.arrival << ' ' << p.duration << endl;
    xs.pop();
  }
}

void show_processes(list<Process> processes)
{
  list<Process> xs = processes;
  cout << "Processes:" << endl;
  while (!xs.empty())
  {
    Process p = xs.front();
    cout << "\tarrival=" << p.arrival << ", duration=" << p.duration
         << ", first_run=" << p.first_run << ", completion=" << p.completion << endl;
    xs.pop_front();
  }
}

list<Process> fifo(pqueue_arrival workload)
{
  list<Process> complete;
  int curtime = 0;
  while (!workload.empty())
  {
    Process p = workload.top();

    if (p.arrival > curtime)
    {
      curtime = p.arrival;
    }

    p.first_run = curtime;
    curtime = curtime + p.duration;
    p.completion = curtime;
    complete.push_back(p);
    workload.pop();
  }

  return complete;
}

list<Process> sjf(pqueue_arrival workload)
{

  list<Process> complete;
  pqueue_duration work2;
  int curtime = 0;
  while ((!workload.empty()))
  {
    Process p = workload.top();
    if (p.arrival > curtime)
    {
      curtime = p.arrival;
    }
    work2.push(p);
    workload.pop();

    Process j = work2.top();
    j.first_run = curtime;
    curtime = curtime + j.duration;
    j.completion = curtime;
    complete.push_back(j);
    work2.pop();
  }
  while ((!work2.empty()))
  {
    Process i = work2.top();
    i.first_run = curtime;
    curtime = curtime + i.duration;
    i.completion = curtime;
    complete.push_back(i);
    work2.pop();
  }

  return complete;
}















list<Process> stcf(pqueue_arrival workload)
{

  list<Process> complete;
  //first make the duration queue and pop first element
  pqueue_duration work2;
  int curtime = 0;
  //place the first element on the stack
  Process l = workload.top();
  if (l.arrival > curtime)
  {
    curtime = l.arrival;
  }
  work2.push(l);
  workload.pop();



  while (!work2.empty() || (!workload.empty()))
  
  {


    // cout << "i was here" << endl;

    // if arrival queue is empty and duration queue is not empty. all the processes have arrived already.
    if ((!work2.empty()) && (workload.empty()))
    {
      // cout << "i ran 4";
      while (!work2.empty())
      {
        // cout << "i ran 5";
        Process p = work2.top();

        cout<<p.first_run<<endl;
        // if(p.arrival>curtime){
        // curtime=p.arrival;
        // }
        


        if (p.first_run<0)
        {
          p.first_run = curtime;
        }
        // cout << p.duration << endl;
        curtime = curtime + p.duration;
        p.completion = curtime;


        complete.push_back(p);
        work2.pop();
      }
      // if arrival queue is not empty and duration queue is empty . place a node from arrival to duration queue.
    }
    else if ((work2.empty()) && (!workload.empty()))
    {
      // cout << "i ran 3";
      Process k = workload.top();
      if (k.arrival > curtime)
      {
        curtime = k.arrival;
      }
      work2.push(k);
      workload.pop();
    }
    else
    {
      // if both are non empty

      Process wrkl = workload.top();
      Process dur = work2.top();

      // cout<<"i am here"<<endl;

      // if a process will interrupt the current process
      if (wrkl.arrival == dur.arrival)
      {
         

        work2.push(wrkl);
        workload.pop();
        cout << "i rant 50" << endl;
        cout << workload.size() << endl;
        continue;
      }

      if ((dur.duration + curtime) >= wrkl.arrival)
      {

        
        int difference = wrkl.arrival - curtime;
        
       
        // go through b
       if (dur.first_run<0)
        {
        work2.pop();
        dur.first_run = curtime;
         dur.duration = dur.duration - difference;
        work2.push(dur);
          

        }
       


       
        
        //changed the duration left of the process. and paused at the time for the second process to come;
        curtime = curtime + difference;

        work2.push(wrkl);
        
        workload.pop();
      }


      else
      {
       

        // go through b
        if (dur.first_run<0)

        {
          dur.first_run = curtime;
        }
        
        curtime = curtime + dur.duration;
        dur.completion = curtime;
        // p.arrival= 0;
        complete.push_back(dur);
        work2.pop();
      }
    }
  }
  return complete;
}







list<Process> rr(pqueue_arrival workload)
{

  list<Process> complete;
  //first make the duration queue and pop first element
  pqueue_arrival work2;
  pqueue_arrival work3;
  int curtime = 0;
  //place the first element on the stack
  Process l = workload.top();
  if (l.arrival > curtime)
  {
    curtime = l.arrival;
  }
  work2.push(l);
  workload.pop();




  while (!work2.empty() || (!workload.empty()))
  {
    

    // if arrival queue is empty and duration queue is not empty. all the processes have arrived already.
    if ((!work2.empty()) && (workload.empty()))
    
    

    {
      cout<<curtime<<endl;
      cout<<work2.size()<<"the size was" <<endl;

      // while (!work2.empty())
      // {

        //push all out of work2 with decrement time, if time ==0 then remove , increment time
        while (!work2.empty())
        {
          Process h = work2.top();
          work2.pop();
          
        if (h.first_run<0){
        h.first_run = curtime;
        }
        h.duration = h.duration - 1;
        work3.push(h);
        curtime = curtime + 1;
        }
        
        
        while (!work3.empty())
        {
          Process d = work3.top();
          work3.pop();
          if (d.duration <= 0)
          {
          
            d.completion = curtime;
            complete.push_back(d);
            
          }
          else
          {
            work2.push(d);

          }
        }
      // }
    }
  
    

    // if arrival queue is not empty and duration queue is empty . place a node from arrival to duration queue.
    else if ((work2.empty()) && (!workload.empty()))
    {
      cout << "i ran 3"<<endl;
      Process k = workload.top();
      if (k.arrival > curtime)
      {
        curtime = k.arrival;
      }
      work2.push(k);
      workload.pop();
    }
    else
    {
      // if both are non empty

      Process wrkl = workload.top();
      Process dur = work2.top();

      // cout<<"i am here"<<endl;

      // if a process will interrupt the current process
      


      if ((1 + curtime) >= wrkl.arrival)
      {
        cout<<"this si fuckd 2"<<endl;
        // run it for one loop and then add the process
        

        //push all out of work2 with decrement time, if time ==0 then remove , increment time
        while (!work2.empty())
        {
          Process y = work2.top();
          work2.pop();
          
        if (y.first_run<0){
        y.first_run = curtime;
        }
        y.duration = y.duration - 1;
        work3.push(y);
        curtime = curtime + 1;
        }
        
        
        while (!work3.empty())
        {
          Process t = work3.top();
          work3.pop();
          if (t.duration <= 0)
          {
            t.completion = curtime;
            complete.push_back(t);
            
          }
          else
          {
            work2.push(t);

          }
        }


      
        // add the node
        //add the  process from arrival queue and pop it off
        work2.push(wrkl);
        workload.pop();
        
        if (wrkl.arrival == workload.top().arrival){
        Process p=workload.top();
        workload.pop();
        work2.push(p);
      }






      }
      else
      {
        cout<<"this si fuckd"<<endl;

      
        // do timestep of 1
        //push all out of work2 with decrement time, if time ==0 then remove , increment time
        while (!work2.empty())
        {
          
          Process j = work2.top();
          work2.pop();
          
        if (j.first_run<0){
        j.first_run = curtime;
        }
        j.duration = j.duration - 1;
        work3.push(j);
                curtime = curtime + 1;
        }
        

        while (!work3.empty())
        {
          Process u = work3.top();
          work3.pop();
          if (u.duration <= 0)
          {
            u.completion = curtime;
            complete.push_back(u);
            
          }
          else
          {
            work2.push(u);

          }
        }
    
      }
      }
    
  }

  // from the arrival queue , first take it off till the next one comes in,
  // then run it, then pop the second node in arrival queue and run 1 second intervals
  //switching through every time.

  return complete;
}

float avg_turnaround(list<Process> processes)
{
  float size = processes.size();
  float avg = 0;
  float sumadded = 0;
  for (auto elem : processes)
  {

    sumadded = elem.completion - elem.arrival;
    avg = avg + sumadded;
    sumadded = 0;
  }
  float a = avg / size;

  return a;
}

float avg_response(list<Process> processes)
{
  float size = processes.size();
  float avg = 0;
  float sumadded = 0;
  for (auto elem : processes)
  {
    sumadded = elem.first_run - elem.arrival;
    avg = avg + sumadded;
    sumadded = 0;
  }
  float a = avg / size;

  return a;
}

void show_metrics(list<Process> processes)
{
  float avg_t = avg_turnaround(processes);
  float avg_r = avg_response(processes);
  show_processes(processes);
  cout << '\n';
  cout << "Average Turnaround Time: " << avg_turnaround(processes) << endl;
  cout << "Average Response Time:   " << avg_response(processes) << endl;
}
