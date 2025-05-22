#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;



int main (){
    char cwd[1024];
    string input;
    while(true){
   
        getcwd(cwd, sizeof(cwd));
        cout<<"    $ ("<<cwd<<")\n";
        cout<<"eve >>";

        getline(cin, input);


        if(input == "exit"){
            break;
        }
        if(input.empty()){

            continue;
        }


        //tokenize
        vector<string> arguments;
        string word = "";
        for(char c: input){
            if(c == ' '){
                arguments.push_back(word);
                word ="";
            }
            else{
                word += c;
            }

        }

        

        if(!word.empty()){
            arguments.push_back(word);
        }


        string command = arguments[0];

        if(command == "cd"){
            const char* path;
            if(arguments.size()>1){
                path = arguments[1].c_str();
            }
            else{
                path = getenv("HOME");
            }

            if(chdir(path)!=0){
                perror("CD failed");
            }
            continue;

        }

        vector<char*> args;

        for(auto& p : arguments){
            args.push_back(&p[0]);
        }

        args.push_back(nullptr);


        pid_t pid = fork();
        // 0 is in the child process
        // pid> 0 means is in the parent process

        if(pid == 0){
            execvp(args[0], args.data());
            perror("Command failed");
            return 1;
        }else if (pid > 0){
            wait(nullptr);
        }
        else{
            perror("fork failed");
        }
        

    }





    return 0;
}