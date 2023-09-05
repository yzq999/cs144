#include "reassembler.hh"
#include "iostream"
using namespace std;



void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring, Writer& output )
{

  capacity_ = output.available_capacity();

  if(output.is_closed()) return;

  int start = max(start_ , first_index);
  int end = min(start_+capacity_ , first_index+data.size());

  if(end < start) return;
  
  if(q.count(start) == 0)
    q.insert({ start , data.substr(start-first_index , end-start)});
  else if(end-start > (int)q[start].size()) 
    q[start] = data.substr(start-first_index , end-start);


  if(q.count(start_) != 0){
    // cout << 2 << endl;
    int tmp_end = start_ + q[start_].size();
    string out = q[start_];
    vector<int> indexs;
    for(auto& tmp:q){
      // cout << tmp.first << ' ' << tmp.second.size()<<endl;
      if(tmp.first > tmp_end) break;
      
      
      if(tmp.first + (int)tmp.second.size() > tmp_end){
        out += tmp.second.substr(tmp_end-tmp.first,tmp.first + tmp.second.size()-tmp_end);
        tmp_end = tmp.first + (int)tmp.second.size();
      }

      indexs.push_back(tmp.first);
      // cout << 4 << endl;
    }
    // cout << out.size() << endl;
    for(auto& index:indexs) q.erase(index);
    start_ = tmp_end;
    output.push(out);
  }
  // cout << "end" << endl;
  if(is_last_substring) is_close = true;
  if(is_close && bytes_pending()==0) output.close();


  (void)first_index;
  (void)data;
  (void)is_last_substring;
  (void)output;
}

uint64_t Reassembler::bytes_pending() const
{
  uint64_t ret = 0;
  uint64_t start = -1, end = -1;
  
  for(auto& tmp:q){
    if((int)end < tmp.first) start = tmp.first , end = start+tmp.second.size() , ret += end-start ;
    else if((int)end >= tmp.first && end<tmp.first+tmp.second.size()) ret += tmp.first+tmp.second.size()-end , end = tmp.first+tmp.second.size() ;
  }

  return ret;
}
