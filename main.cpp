#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;



int main (){
    //variables
    char cwd[1024];
    string input;

    //Loop for the terminal
    while(true){


        //Get current directory
        getcwd(cwd, sizeof(cwd));

        //show current directory
        cout<<"    $ ("<<cwd<<")\n";
        cout<<"eve >>";

        //get the command from the user
        getline(cin, input);


        if(input == "exit"){
            break;
        }

        //if the user doesn't enter anything, repeat the loop
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

        
    //if the word still have something, save it in the vector
        if(!word.empty()){
            arguments.push_back(word);
        }

        //get the command, which will be the first word of what user entered
        string command = arguments[0];


        //Manage manually cd
        if(command == "cd"){
            //variable to save the path
            const char* path;
            //if there are something after cd, it will be the path, if it doesn't, it will get a defaul value ("HOME")
            if(arguments.size()>1){
                path = arguments[1].c_str();
            }
                
            else{
                path = getenv("HOME");
            }
            //if cd gets a problem
            if(chdir(path)!=0){
                perror("CD failed");
            }
            continue;

        }

        //Transform the vector into the char* vectors
        vector<char*> args;

        for(auto& p : arguments){
            args.push_back(&p[0]);
        }

        //Add nullptr at the end of the vector, because if it doesn't it will be a infinite loop
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
