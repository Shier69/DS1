#include <iostream> 
#include <fstream>
#include <string>
#include <cstdlib> 
#include <vector> 

using namespace std;

struct detail {
	
	string schoolName;
	string departmentName;
	string day;
	string level;
	int student;
	int teacher;
	int graduated;
	
};

void printAll( vector<detail> list ) {
	
	detail job;
	
	for( int i=0; i<list.size(); i++ ) { 
		job=list.at(i);
		printf( "[%d]%d\n", i, job.graduated );
	}
	
}

bool loading( vector<detail> &list ) {
	
	fstream	inFile;
	string fileID;
	detail file;
	
	cout << endl << "please input a file number:\n";
	cin >> fileID;
	fileID="input"+fileID+".txt";
	inFile.open( fileID.c_str(), fstream::in );
	
	if ( !inFile.is_open() ) {
		cout << endl << fileID << "does not exist!\n";
		return false;
	} // if 
	else {
		char cstr[255];
		int fNo, pre, pos;
		
		inFile.getline( cstr, 255, '\n' ); 
		inFile.getline( cstr, 255, '\n' ); 
		inFile.getline( cstr, 255, '\n' ); 
		
		while( inFile.getline( cstr, 255, '\n' ) ) {
			string buf, cut;
			fNo=0, pre=0;
			buf.assign( cstr );
			
			while( pos > 0 && fNo < 10 ) {
				pos=buf.find_first_of( '\t', pre ); 			// find where the next tab is  
				cut=buf.substr( pre, pos-pre ); 
					
				switch(++fNo) {
					case 2: file.schoolName=cut; 
						break;
					case 4: file.departmentName=cut;
						break;
					case 5: file.day=cut;
						break;
					case 6: file.level=cut;
						break; 
					case 7: file.student=atoi(cut.c_str());
						break;
					case 8: file.teacher=atoi(cut.c_str());
						break;
					case 9: file.graduated=atoi(cut.c_str());
						break;
				} // switch		
				pre=pos+1;
			} // while
			list.push_back(file);
		} // while
		inFile.close();
	} // else 
	
	if ( list.size()==0 ) {
		cout << "there is nothing from the file.\n" << endl; 
		return false;
	} // if
	
} // loading()

int main() {
	
	vector<detail> list;
	
	loading( list );
	printAll( list );
	
}
