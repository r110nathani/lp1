#include <bits/stdc++.h>
using namespace std;

class MNTField
{
    string name;
    int PP;
    int KP;
    int MDTPTR;
    int KDPTR;

    MNTField()
    {
        name = "";
        PP = 0;
        KP = 0;
        MDTPTR = 0;
        KDPTR = 0;
    }
    friend class Pass2;
};

class APTAB
{
    vector<pair<string, string>> APTAB;
    friend class Pass2;
};

vector<string> tokenizer(string s)
{
    vector<string> res;
    string word = "";
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] != ' ')
        {
            word += s[i];
        }
        else
        {
            res.push_back(word);
            word = "";
        }
    }
    res.push_back(word);
    return res;
}

class Pass2
{
    unordered_map<int, vector<string>> MDT;
    vector<pair<MNTField, APTAB>> MNT;
    unordered_map<string, string> KPTAB;
    fstream MDTfile;
    fstream MNTfile;
    fstream KPTABfile;
    fstream file;
    fstream APTABfile;
    vector<string> paramters;

public:
    Pass2()
    {
        MDTfile.open("MDT.txt", ios::in);
        if (MDTfile.is_open())
        {
            getMDT();
        }
        else
        {
            cout << "Error opening MDT file !" << endl;
        }

        MNTfile.open("MNT.txt", ios::in);
        if (MNTfile.is_open())
        {
            getMNT();
        }
        else
        {
            cout << "Error opening MNT file!" << endl;
        }

        KPTABfile.open("KPTAB.txt", ios::in);
        if (KPTABfile.is_open())
        {
            getKPTAB();
        }
        else
        {
            cout << "Error opening KPTAB file !" << endl;
        }

        APTABfile.open("APTAB.txt", ios::in);
        if (APTABfile.is_open())
        {
            getAPTAB();
        }
        else
        {
            cout << "Error opening APTAB file !" << endl;
        }
    }

    void getKPTAB()
    {
        string line;
        while (getline(KPTABfile, line))
        {
            vector<string> words = tokenizer(line);
            for (int i = 0; i < words.size(); i++)
            {
                KPTAB.insert({words[0], words[1]});
            }
        }
    }

    void getAPTAB()
    {
        string line;
        while (getline(APTABfile, line))
        {
            vector<string> words = tokenizer(line);
            paramters.push_back(words[0]);
        }
    }

    void getMNT()
    {
        // name PP KP KPDTP MDTPTR
        string line;
        while (getline(MNTfile, line))
        {
            vector<string> words = tokenizer(line);
            MNTField fields;
            fields.name = words[0];
            fields.PP = stoi(words[1]);
            fields.KP = stoi(words[2]);
            fields.KDPTR = stoi(words[3]);
            fields.MDTPTR = stoi(words[4]);
            MNT.push_back({fields, APTAB()});
        }
    }

    void getMDT()
    {
        string line;
        while (getline(MDTfile, line))
        {
            vector<string> words = tokenizer(line);
            int ind = stoi(words[0]); // converted linenumber to int
            // next words will be macro words including parameters represented as #
            vector<string> w;
            for (int i = 1; i < words.size(); i++)
            {
                w.push_back(words[i]);
            }
            MDT.insert({ind, w});
        }
    }

    void printAll()
    {
        cout << "MNT :" << MNT.size() << endl;
        cout << "MDT :" << MDT.size() << endl;
        cout << "KPTAB :" << KPTAB.size() << endl;
    }

    void callMacro(vector<string> Macrocall)
    {
        // name PP KP KPDTP MDTPTR
        //  file.seekg(postn,ios::beg);
        // for(int i=0;i<Macrocall.size();i++){
        //     cout<<Macrocall[i]<<"|";
        // }
        // cout<<endl;
        APTAB aptab;
        string mName = Macrocall[0];
        int ind = findInMNT(mName);
        int pp = MNT[ind].first.PP;
        int kp = MNT[ind].first.KP;
        int mdtPTR = MNT[ind].first.MDTPTR;
        int kpPTR = MNT[ind].first.KDPTR;
        // get all parameter names with #index into aptab
        int p = 0;
        while (paramters[p] != mName)
            p++;
        p++;
        for (int i = 0; i < pp + kp; i++)
        {
            aptab.APTAB.push_back({paramters[p], ""});
            p++;
        }
        // assigning default values to key word
        for (int i = 0; i < aptab.APTAB.size(); i++)
        {
            if (KPTAB.find(aptab.APTAB[i].first) != KPTAB.end())
            {
                aptab.APTAB[i].second = KPTAB[aptab.APTAB[i].first];
            }
        }
        // mapped pp parameters to its values
        for (int i = 0; i < pp; i++)
        {
            aptab.APTAB[i].second = Macrocall[i + 1];
        }
        // cout << "pp mapped" << endl;
        // map keyword values if any provided
        int j = pp + 1;
        // cout<<j<<endl;
        while (j < Macrocall.size())
        {
            string param = Macrocall[j];
            string paramName = "";
            string paramValue = "";
            int l = 0;
            while (l < param.size())
            {
                if (param[l] != '=')
                {
                    paramName += param[l];
                }
                else
                    break;
                l++;
            }
            l++;
            while (l < param.size())
            {
                paramValue += param[l];
                l++;
            }
            int ind = findInAptab(aptab, paramName);
            aptab.APTAB[ind].second = paramValue;
            j++;
        }
        // cout << "keyword mapped" << endl;
        // for (int i = 0; i < aptab.APTAB.size(); i++)
        // {
        //     cout << aptab.APTAB[i].first << "|" << aptab.APTAB[i].second << endl;
        // }

        while (mdtPTR < MDT.size())
        {
            string topush = "+";
            vector<string> line = MDT[mdtPTR++];
            if (line[0] == "MEND")
                return;
            else if (findInMNT(line[0]) != -1)  //handle macro within macro
            {
                callMacro(line);
            }
            else
            {
                for (int i = 0; i < line.size(); i++)
                {
                    if (line[i][0] == '#')
                    {
                        int pno = line[i][1] - '0';
                        topush += aptab.APTAB[pno - 1].second + " ";
                    }
                    else
                    {
                        topush += line[i] + " ";
                    }
                }
                cout << topush << endl;
            }
        }
    }

    void Pas2()
    {

        file.open("Pass2.txt", ios::in);
        string line;
        while (getline(file, line))
        {
            vector<string> words = tokenizer(line);
            if (findInMNT(words[0]) != -1)
            {
                // macro call
                callMacro(words);
            }
            else
            {
                cout << line << endl;
            }
        }
    }

    int findInMNT(string w)
    {
        for (int i = 0; i < MNT.size(); i++)
        {
            if (MNT[i].first.name == w)
                return i;
        }
        return -1;
    }

    int findInAptab(APTAB a, string name)
    {
        for (int i = 0; i < a.APTAB.size(); i++)
        {
            if (name == a.APTAB[i].first)
                return i;
        }
        return -1;
    }
};

int main()
{
    Pass2 p;
    p.printAll();
    p.Pas2();
}