#include <string>
#include <stack>

using namespace std;

bool isValid(string input) {
    stack<char> st;
    char x;
    for(int i=0; i<input.length(); i++){
      if((input[i]!='(')||(input[i]!=')')||(input[i]!='{')||(input[i]!='}')||(input[i]!='[')||(input[i]!=']')){
        input[i]='\0';
      }
    }
    for(int i=0; i<input.length(); i++){
      if(input[i]=='(' || input[i]=='[' ||input[i]=='{'){
        st.push(input[i]);
        continue;
      }
      if(st.empty()){
        return false;
      }
      switch (input[i]){
        case ')':
        x=st.top();
        st.pop();
        if(x=='{' || x=='[')
        return false;
        break;
        case '}':
        x=st.top();
        st.pop();
        if(x=='('||x=='[')
        return false;
        break;
        case ']':
        x=st.top();
        st.pop();
        if(x=='('||x=='{')
        return false;
        break;
      }
    }
    if(!st.empty()){
      return false;
    }
    return true;    // modify this!
}
