#include<bits/stdc++.h>
using namespace std;

class Assembler
{
    public:
    unordered_map<string,pair<string,int>> OPTAB;
    unordered_map<string,int> REG;
    unordered_map<string,int> CONDCS;

    vector<pair<string,int>> LITTAB;
    vector<pair<string,int>> SYMTAB;

    bool activepool;
    int lc;


    Assembler()
    {
        OPTAB["STOP"] = {"IS", 0};
		OPTAB["ADD"] = {"IS", 1};
		OPTAB["SUB"] = {"IS", 2};
		OPTAB["MULT"] = {"IS", 3};
		OPTAB["MOVER"] = {"IS", 4};
		OPTAB["MOVEM"] = {"IS", 5};
		OPTAB["COMP"] = {"IS", 6};
		OPTAB["BC"] = {"IS", 7};
		OPTAB["DIV"] = {"IS", 8};
		OPTAB["READ"] = {"IS", 9};
		OPTAB["PRINT"] = {"IS", 10};
		OPTAB["START"] = {"AD", 1};
		OPTAB["END"] = {"AD", 2};
		OPTAB["ORIGIN"] = {"AD", 3};
		OPTAB["EQU"] = {"AD", 4};
		OPTAB["LTORG"] = {"AD", 5};
		OPTAB["DC"] = {"DL", 1};
		OPTAB["DS"] = {"DL", 2};

		REG["AREG"] = 1;
		REG["BREG"] = 2;
		REG["CREG"] = 3;
		REG["DREG"] = 4;

		CONDCS["LT"] = 1;
		CONDCS["LE"] = 2;
		CONDCS["EQ"] = 3;
		CONDCS["GT"] = 4;
		CONDCS["GE"] = 5;
		CONDCS["ANY"] = 6;

        activepool = true;
        lc = 0;
    }

    vector<string> extract_words(string s)
    {
        string temp = "";
        vector<string> words;
        for(int i = 0 ; i<s.size() ; i++)
        {
            if(s[i] != ' ')
            {
                temp += s[i];
            }

            else
            {
                words.push_back(temp);
                temp = "";
            }
        }
        words.push_back(temp);
        return words;
    }

    int find_in_literal_table(string s)
    {
        for(int i = 0; i<LITTAB.size() ; i++)
        {
            if(LITTAB[i].first == s) return i+1;
        }
        return -1;
    }

    int find_in_symbol_table(string s)
    {
        for(int i = 0; i<SYMTAB.size() ; i++)
        {
            if(SYMTAB[i].first == s) return i+1;
        }
        return -1;
    }

    void handle_literal(string s)
    {
        if(activepool)
        {
            if(find_in_literal_table(s) == -1)
            {
                LITTAB.push_back({s,-1});
            }
            cout<<"(L,"<<find_in_literal_table(s)<<") ";
        }

        else
        {
            activepool = true;
            int i = 0;
            while(i<LITTAB.size())
            {
                if(LITTAB[i].second == -1)
                {
                    break;
                }
            }

            while(i<LITTAB.size())
            {
                LITTAB[i].second = lc;
                lc++;
                i++;
            }
            lc--;
        }
    }

    void printOp(string s)
    {
        cout<<"("<<OPTAB[s].first<<","<<OPTAB[s].second<<") ";
    }

    void printReg(string s)
    {
        cout<<"("<<REG[s]<<") ";
    }

    void printCon(string s)
    {
        cout<<"("<<CONDCS[s]<<") ";
    }

   void print_literal_table(){
        cout<<"Sr"<<setw(15)<<"Name"<<"Addrs"<<endl;
        for(int i=0;i<LITTAB.size();i++){
            cout<<i<<setw(15)<<LITTAB[i].first<<setw(15)<<LITTAB[i].second<<endl;
        }
    }

    void print_symbol_table(){
        cout<<"Sr"<<setw(15)<<"Name"<<"Addrs"<<endl;
        for(int i=0;i<SYMTAB.size();i++){
            cout<<i<<setw(15)<<SYMTAB[i].first<<setw(15)<<SYMTAB[i].second<<endl;
        }
    }

    void size_one(vector<string>words)
    {
        printOp(words[0]);
        if(words[0] == "LTORG")
        {
            activepool = false;
            handle_literal("");
        }
    }

    void size_two(vector<string>words)
    {
        printOp(words[0]);
        if(words[0] == "ORIGIN")
        {
            int k = find_in_symbol_table(words[1]);
            lc = SYMTAB[k].second;
            cout<<"(S,"<<k+1<<") ";
        }

        else
        {
            if(find_in_symbol_table(words[1]) == -1)
            {
                SYMTAB.push_back({words[1],-1});
            }
            cout<<"(S,"<<find_in_symbol_table(words[1])<<")"<<" ";
        }

    }
    void size_three(vector<string>words)
    {
        printOp(words[0]);
        if(REG.find(words[1]) != REG.end())
        {
            printReg(words[1]);
        }

        else if(CONDCS.find(words[1]) != CONDCS.end())
        {
            printCon(words[1]);
        }

        if(REG.find(words[2]) != REG.end())
        {
            printReg(words[2]);
        }

        else if(words[2].at(0) == '=') //literal
        {
            handle_literal(words[2]);
        }

        else 
        {
            if(find_in_symbol_table(words[2]) == -1)
            {
                SYMTAB.push_back({words[2],-1});
                cout<<"(S,"<<SYMTAB.size()<<")"<<" ";
            }
            else
            {
                cout<<"(S,"<<find_in_symbol_table(words[2])<<")"<<" ";   
            }
        }

    }

    void passOne()
    {
        string line;
        fstream file;
        file.open("D:/folder for lp1/ASSEMBLER PASS1/sampleinput.txt" , ios :: in);
        if(!file.is_open())cout<<"ERROR"<<endl;
        while(getline(file,line)) 
        {
           // cout<<"E2";
            vector<string> words = extract_words(line);
            if(words[0] == "START")
            {
                printOp("START");
                lc = stoi(words[1]);
                cout<<"(C,"<<lc<<") ";
            }

            else if(words[0] == "END")
            {
                activepool = false;
                handle_literal("");
            }

            else if(OPTAB.find(words[0]) != OPTAB.end())
            {
                if(words.size() == 1) size_one(words);
                else if(words.size() == 2) size_two(words);
                else if(words.size() == 3) size_three(words);
            }

            else
            {
                //symbol
                cout<<lc<<" ";
                if(find_in_symbol_table(words[0]) == -1)
                {
                    SYMTAB.push_back({words[0],lc});
                }
                cout<<"(S,"<<find_in_symbol_table(words[0])<<")"<<" ";

                if(words.size() == 2)
                {
                    size_one({words[1]});
                }
                else{
                    if(words[2].at(0) >= '0' && words[2].at(0) <= '9')
                    {
                        if(find_in_symbol_table(words[0]) == -1)
                        {
                            SYMTAB.push_back({words[0] , lc});
                        }

                        else
                        {
                            int k = find_in_symbol_table(words[0]);
                            SYMTAB[k].second = lc;
                            printOp(words[1]);
                            cout<<"(C,"<<words[2]<<")"<<" ";
                        }
                    }

                    else
                    {
                        if(words[1] == "EQU")
                        {
                            printOp(words[1]);
                            SYMTAB[find_in_symbol_table(words[0])-1].second =  SYMTAB[find_in_symbol_table(words[2])-1].second;
                            cout<<"(S,"<<find_in_symbol_table(words[2])<<") ";
                        }

                        else
                        {
                            vector<string> withoutSymb;
                            for(int i = 1 ; i<words.size() ; i++)
                            {
                                withoutSymb.push_back(words[i]);
                            }

                            if(withoutSymb.size() == 1)size_one(withoutSymb);
                            if(withoutSymb.size() == 2)size_two(withoutSymb);
                            if(withoutSymb.size() == 3)size_three(withoutSymb);
                        
                        }
                    }
                }
            }
            lc++;
            cout<<endl;
        }  
    }
};

int main()
{
    Assembler a;
    a.passOne();
    cout<<endl;
    a.print_literal_table();
    cout<<endl;
    a.print_symbol_table();
    cout<<endl;

}

