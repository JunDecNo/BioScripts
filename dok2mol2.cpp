/*
Author: XU Ximing
Email : ximingxu@qq.com
WeChat: mmcilia520

To compile this cpp program: g++ -std=c++11 dok2mol2.cpp -o dok2mol2
*/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
using namespace std;
std::string& trim(std::string &s){  
    if (s.empty()){  
        return s;  
    }
    s.erase(0,s.find_first_not_of(" "));  
    s.erase(s.find_last_not_of(" ") + 1);  
    return s;  
}  

struct Atom {
	int atomid;
	string atomn;
	float atomx;
	float atomy;
	float atomz;
};

vector<Atom> atoms;
Atom atomi;
int main(int argc, char** argv){
	//USAGE
	if(argc != 4){
		cout<<"USAGE: dok2mol2 result.dok original.mol2 posenumber"<<endl;
		return 0;
	}
	//OK we start to read dok file, and store the atom coordinates
	ifstream in(argv[1]);
	bool copyprint=0;
	string s;
	string header="REMARK Cluster";
	stringstream poseheader;
	poseheader<<header<<setw(4)<<right<<argv[3];
	//cout << "BAD : "<<poseheader.str()<<endl;
	while(getline(in,s)){
		if (s.substr(0,18) == poseheader.str()){
			cout<<"#"<<s.c_str()<<endl;
			copyprint=1;
		}
		else if (s == "END"){
			copyprint=0;
			//cout<<s.c_str()<<endl;

		}
		else if (copyprint ==1) {
			//cout<<s.c_str()<<endl;
			if(s.substr(0,4) == "ATOM"){
				atomi.atomid=stoi(s.substr(6,5).c_str());
				string atom_reducedname=s.substr(12,4).c_str();
				trim(atom_reducedname);
				atomi.atomn=atom_reducedname.substr(0,1);
				atomi.atomx =atof(s.substr(30,8).c_str());
				atomi.atomy =atof(s.substr(38,8).c_str());
				atomi.atomz =atof(s.substr(46,8).c_str());
				atoms.push_back(atomi);
			}
		}
	}
	//OK finish the dok. Next, lets have a look at reference mol2 file
	
	ifstream mol2(argv[2]);
	copyprint=0;
	string mol2line;
	int i=0;
	while(getline(mol2,mol2line)){
		if(mol2line.substr(0,13)== "@<TRIPOS>ATOM"){
			cout<<mol2line.c_str()<<endl;
			copyprint=1;
		}
		else if(mol2line.substr(0,13)== "@<TRIPOS>BOND"){
			copyprint=0;
			cout<<mol2line.c_str()<<endl;
		}
		else if (copyprint==1){
			string atom_inmol2=mol2line.substr(7,6).c_str();
			trim(atom_inmol2);
			if(atoms[i].atomn == atom_inmol2.substr(0,1)){
				cout<<mol2line.substr(0,14).c_str()
				<<setw(12)<<setiosflags(ios::fixed)<<setprecision(4)<<atoms[i].atomx
				<<setw(10)<<setiosflags(ios::fixed)<<setprecision(4)<<atoms[i].atomy
				<<setw(10)<<setiosflags(ios::fixed)<<setprecision(4)<<atoms[i].atomz
				<<mol2line.substr(46).c_str()<<endl;
				//cout <<setw(7)<<atoms[i].atomid<<endl;
				i++;

			}
			else {
				cout<<argv[2]<<": is not the reference mol2 file! Or a bad line in mol2 file for atom number: "<<i + 1<<endl;
				return 0;
			}
			
		}
		else {
			cout<<mol2line.c_str()<<endl;
		}
	}
		//cout<<s.substr(0,14)<<endl;
	
	//cout <<atoms[0].atomn<<endl;
	return 0;
}