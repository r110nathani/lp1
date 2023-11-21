#include<bits/stdc++.h>
using namespace std;

class MNT
{
    friend class Macro;
    string macro_name;
    int pp;
    int kp;
    int mdtp;
    int kptp;

    MNT()
    {
        pp = 0;
        kp = 0;
        mdtp = 0;
        kptp = 0;
    }
};

class PNTB
{
    friend class Macro;
    unordered_map<string,int> PNTAB;
};

class Macro
{
    public:
    unordered_map<int,string> KPTAB;
    unordered_map<int,string> MDT;
    unordered_map<string,pair<MNT,PNTB>> ans;
    int kptab_pointer;
    int mdt_pointer;
    fstream file;
    int MDT_PTR;

    Macro()
    {
        kptab_pointer = 0;
        mdt_pointer = 0;
        file.open("D:/folder for lp1/MACRO PASS1/sample.txt" , ios :: in);
        MDT_PTR = 0;
    }

    vector<string> extract_words(string s)
    {
        string temp = "";
        vector<string> ans;
        for(int i = 0 ; i<s.size() ; i++)
        {
            if(s[i] != ' ')
            {
                temp += s[i];
            }

            else
            {
                ans.push_back(temp);
                temp = "";
            }
        }
        ans.push_back(temp);
        return ans;
    }

    int solve()
    {
        bool macro_found = true;
        string line;
        if(!file.is_open())cout<<"FILE NOT OPENED"<<endl;
        PNTB p1;
        while(getline(file,line))
        {
            vector<string> words = extract_words(line);
            if(macro_found)
            {
                macro_found = false;
                //PNTB p1;
                MNT m1;
                m1.macro_name = words[0];
                int reference_count_pp = 0;
                int reference_count_kp = 0;
               
                for(int i = 1 ; i<words.size() ; i++)
                {
                    string parameter ="";
                    string value = "";
                    bool keyword = false; // to check whether it is keyword or parameter
                    for(int j = 0 ; j<words[i].size() ; j++)
                    {
                        if(words[i][j] != '=' && keyword == false)
                        {
                            parameter += words[i][j];
                        }

                        else
                        {
                            keyword = true;
                            value += words[i][j];
                        }
                    }

                    p1.PNTAB.insert({parameter,p1.PNTAB.size()+1});
                    if(!keyword) 
                    {
                        reference_count_pp++;
                    }

                    else
                    {
                        reference_count_kp++;
                        KPTAB.insert({KPTAB.size()+1 , value});
                    }   
                }
                

                m1.pp = reference_count_pp;
                m1.kp = reference_count_kp;
                m1.kptp = KPTAB.size() - reference_count_kp + 1;
                m1.mdtp = MDT_PTR;                                              //watchout for 
                ans.insert({m1.macro_name , {m1,p1}});
            }

            else
            {
                string topushLine="";
                for(int i=0;i<words.size();i++){
                if(p1.PNTAB[words[i]] != 0){
                        topushLine+="(P,"+to_string(p1.PNTAB[words[i]])+") ";
                    }
                    else{
                        topushLine+=words[i]+" ";
                    }
                }
                //cout<<topushLine<<endl;
                MDT.insert({MDT_PTR,topushLine});
                MDT_PTR++;
            }

            if(words[0] == "MEND") return 1;
        }

        cout<<"Error returning from Handle Macro"<<endl;
        return -1;
    }

    void print_MNT()
    {
        cout<<"M-name\tpp\tkp\tmdtp\tkptp"<<endl;
        for(auto it : ans)
        {
            MNT m = it.second.first;
            cout<<m.macro_name<<"\t"<<m.pp<<"\t"<<m.kp<<"\t"<<m.mdtp<<"\t"<<m.kptp<<endl;
        }
    }

    void print_PNTAB()
    {
        for(auto it : ans)
        {
            PNTB p = it.second.second;
            for(auto it : p.PNTAB)
            {
                cout<<it.first<<" "<<it.second<<endl;
            }
        }
    }

    void print_MDT()
    {
        for(auto &it :MDT)
        {
            cout<<it.second<<endl;
        }
    }

};

int main()
{
    Macro m;
    m.solve();
    m.print_PNTAB();
    m.print_MDT();
}