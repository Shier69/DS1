// DS1#2_06_10727108�\�v��_10727125�ۼ}�� 

#include<iostream>
#include<string>
#include<cstdio>

using namespace std;

typedef enum { Copy=-1, Right,Down,Left,Up} Moves;
// ��ܨC�@�B����V 

struct Judge {
	bool up;
	bool down;
	bool left;
	bool right;
	char ch;
	int way;
};

class CoXY{
	
	int x; // X�b 
	int y; // Y�b 
	
public:
	CoXY(): x(0), y(0) {
	}; 
	
	CoXY( int px, int py){
		x = px;
		y = py;
	}
	
	CoXY( const CoXY& pt): x(pt.x), y(pt.y) {
	}
	
	void setXY( const int px, const int py ) {
		x=px;
		y=py;
	}
	
	int getX() const {
	    return x;
	}
	
	int getY() const {
	    return y;
	}
	
};

class Maze{


public :
	
	int maxX, maxY;			// �Ψ�Ū�g�c�����e 
	CoXY coMax;				// ��XY�s��g�c������ 
	CoXY nowXY;				// �ѹ��{�b��m 
	CoXY branch;			// �ѹ��J��ø���  �����y�Ц�m 
	CoXY goal;				// ��ѹ��n�䪺�ؼФj��@�Ӯ�  ����쪺���Ӭ����y�� 
	Judge **matrix; 		// �s��R���g�c 
	Judge **visitedMatrix;  // �s��V���g�c 
	
	void matrixBackup() {	// ��V�g�c�ŵۮ�  �NR�g�c����T�ƻs��V�W 
		for( int a=0; a<coMax.getY(); a++ ){
			for( int b=0; b<coMax.getX(); b++){
				visitedMatrix[a][b].ch = matrix[a][b].ch ;
			}
		}
	}
	
	bool visitedExistE() {  // �P�_V�g�c�O�_�٦�E�s�b  �S����ܨ����F�o�S���ؼ� 
		
		for( int a=0; a<coMax.getY(); a++ ){
			for( int b=0; b<coMax.getX(); b++){
				if( visitedMatrix[a][b].ch =='E')
					return true ;
			}
		}
		
		return false ;
		
	} 
	
	void clr() {  // �N�g�c��l�� 
		
		for( int a=0; a<coMax.getY(); a++ ){
			for( int b=0; b<coMax.getX(); b++){
				if ( matrix[a][b].ch == 'V' )
					matrix[a][b].ch = 'E' ;
			}
		}
		
	}
	
	void matrixToVisited() { // �N�ѹ��Ҩ�����copy��V�g�c 
	
		for( int a=0; a<coMax.getY(); a++) {
			for( int b=0; b<coMax.getX(); b++){
				if( matrix[a][b].ch == 'V' ) 
					visitedMatrix[a][b].ch = 'V' ;
			}
		}	
	
	}
	
	void Initialize(){ // ��l�ƨC�@���m���W�U���k 
		
		for( int a=0; a<maxY; a++ ){
			for( int b=0; b<maxX; b++){
				matrix[a][b].up = true ;    //���W�� 
				matrix[a][b].down = true ;  //���U�� 
				matrix[a][b].right = true ; //���k�� 
				matrix[a][b].left = true ;  //�������� 
				matrix[a][b].way = 4;       //�C�@�榳4���ø� 
				
			}
		}
		
	}	
	
	bool LoadFile( string fileName ) {      // Ū�ɮ� 
	
		FILE *infile = NULL ;
		bool success = false ;
		char temp='\0', ch ;
	
		fileName = "input" + fileName + ".txt" ;
		infile = fopen( fileName.c_str(), "r") ;
	
		if ( infile == NULL )                   // �P�_�O�����ɦW 
			cout << fileName << "does not exist!" << endl ;
		else {
			
			fscanf( infile, "%d %d", &maxX, &maxY ) ; // Ū�J�ɮפ����Ʀr�]�w�g�c�����P�e 
			fscanf( infile, "%c", &ch ) ;             // Ū�J�ɮפ����g�c 
			coMax.setXY( maxX, maxY ) ;				  // �]�w�g�c�����e 
			matrix = new Judge*[ coMax.getY()+1];	  // �]�m�ʺA�}�C  Y�b 
			visitedMatrix = new Judge*[ coMax.getY()+1];
			
			for( int a=0; a<coMax.getY(); a++) {	  // �]�m�ʺA�}�C X�b 
				matrix[a] = new Judge[ coMax.getX()] ;
				visitedMatrix[a] = new Judge[ coMax.getX()] ;
			}
		
			if ( coMax.getX() > 0 && coMax.getY() > 0 ) {
				int nowLine=0;						  // �ثeY�b 

				for( int a=0; a<coMax.getY(); a++ ){
					temp='\0';
					for( int b=0; b<coMax.getX() && temp != '\n' ; b++ ) {
						fscanf( infile, "%c ", &temp ) ;
						if ( temp!='\n') {
							matrix[a][b].ch = temp;
						}
					}
					nowLine++;
				}
				if ( nowLine == coMax.getY() )
					success = true;
			}
			fclose(infile) ;
		} 
		return success ;
	}
	
	void SetObstacle() {  // �]�w��ê 
		
		
		for( int a=0; a<coMax.getY(); a++ ){
			for( int b=0; b<coMax.getX(); b++ ){

				if( a==0 ){                // �g�c�W����ɵL�k���W�� 
					matrix[a][b].up=false;
					matrix[a][b].way--;
				}
					
				if( b==0 ){                 // �g�c������ɵL�k������  
			        matrix[a][b].left=false;
			        matrix[a][b].way--;
			    }
					
				if( a==coMax.getY()-1 ){     // �g�c�U�������ɵL�k���U��  
					matrix[a][b].down=false;
					matrix[a][b].way--;
			    }
			    
				if( b==coMax.getX()-1 ) {     // �g�c�k�������ɵL�k���k��  
					matrix[a][b].right=false;
					matrix[a][b].way--;
				}
				
				if( matrix[a][b].ch =='O' ) {    //�J���ê�� 
					if( a==0 && b==0 ){          //�g�c���W���L�k������,�W�訫 
						matrix[a+1][b].up=false ;
						matrix[a+1][b].way--; 
						matrix[a][b+1].left=false ;
						matrix[a][b+1].way--;						
					}
					else if( a==0 && b==coMax.getX()-1 ) {  //�g�c�k�W���L�k�����k,�W�訫 
						matrix[a][b-1].right=false;
						matrix[a][b-1].way--;
						matrix[a+1][b].up=false;	
						matrix[a+1][b].way--;
					}
					else if( a==coMax.getY()-1 && b==coMax.getX()-1 ) {  //�g�c�k�U���L�k�����k,�U�訫 
						matrix[a-1][b].down=false;
						matrix[a-1][b].way--;
						matrix[a][b-1].right=false;
						matrix[a][b-1].way--;
					}
					else if( a==coMax.getY()-1 && b==0 ) {  //�g�c���U���L�k������,�U�訫 
						matrix[a-1][b].down=false;
						matrix[a-1][b].way--;
						matrix[a][b+1].left=false;
						matrix[a][b+1].way--;
					}
					else if ( a==0 ) {  // ��Y�b��0 ��ܬ��g�c�W�t ��W��L���i�� 
						matrix[a+1][b].up=false ;
						matrix[a+1][b].way--;
						matrix[a][b+1].left=false ;
						matrix[a][b+1].way--;	
						matrix[a][b-1].right=false;
						matrix[a][b-1].way--;
					}
					else if ( b==0 ) { // ��X�b��0 ��ܥ��䬰�g�c��t �䥪��L���i�� 
						matrix[a+1][b].up=false ;
						matrix[a+1][b].way--; 
						matrix[a][b+1].left=false ;
						matrix[a][b+1].way--;
						matrix[a-1][b].down=false;
						matrix[a-1][b].way--;
					}
					else if ( a==coMax.getY()-1 ) { // ��Y�b���g�c���� ��ܤU�謰�g�c�U�t ��U��L���i�� 
						matrix[a-1][b].down=false;
						matrix[a-1][b].way--;
						matrix[a][b+1].left=false;
						matrix[a][b+1].way--;
						matrix[a][b-1].right=false;
						matrix[a][b-1].way--;
					}
					else if ( b==coMax.getX()-1) { // ��X�b���g�c���e ��ܥk�謰�g�c��t ��k��L���i�� 
						matrix[a-1][b].down=false;
						matrix[a-1][b].way--;
						matrix[a][b-1].right=false;
						matrix[a][b-1].way--;
						matrix[a+1][b].up=false ;
						matrix[a+1][b].way--;
					}
					else { // �Y�ثe��l�����g�c��t ��ܨ䬰�g�c���� 
						matrix[a-1][b].down=false;
						matrix[a-1][b].way--;
						matrix[a][b-1].right=false;
						matrix[a][b-1].way--;
						matrix[a+1][b].up=false ;
						matrix[a+1][b].way--;
						matrix[a][b+1].left=false;
						matrix[a][b+1].way--;
					}
				}
			}
		}
	}

	void SetDeadRoad() {  // �J��ø��� ������ �N�����]������ 
		
		if ( matrix[branch.getY()][branch.getX()].right )
			matrix[branch.getY()][branch.getX()].right=false;
		else if( matrix[branch.getY()][branch.getX()].down )
			matrix[branch.getY()][branch.getX()].down=false;
		else if( matrix[branch.getY()][branch.getX()].left )
			matrix[branch.getY()][branch.getX()].left=false;
		else if( matrix[branch.getY()][branch.getX()].up )
			matrix[branch.getY()][branch.getX()].up=false;
		
		matrix[branch.getY()][branch.getX()].way--;
	}

	void NextStep() {														// �ѹ��������Ǭ� �k�U���W 
		
		if ( nowXY.getX()==0 && nowXY.getY()==0 && matrix[0][0].ch == 'E' ) // �ѹ��_�l�I��V 
			matrix[nowXY.getY()][nowXY.getX()].ch = 'V' ;
		
		if ( matrix[nowXY.getY()][nowXY.getX()].way!=1 )                    // ��ѹ��P�w�i������V���u1���Хܬ��ø� 
			branch.setXY( nowXY.getX(), nowXY.getY() ) ; 
		
		if( matrix[nowXY.getY()][nowXY.getX()].right ) {					// �Y�ѹ��i���k�� 
 			nowXY.setXY( nowXY.getX()+1, nowXY.getY() ) ;					// ���ʫ���ܨ�y�� 
 			if ( matrix[nowXY.getY()][nowXY.getX()].ch == 'G' )				// �Y�J��Goal �N��y�аO�U 
 				goal.setXY( nowXY.getX(), nowXY.getY() ) ;
				
			if (matrix[nowXY.getY()][nowXY.getX()].left && matrix[nowXY.getY()][nowXY.getX()].ch != 'G' ) { 
				matrix[nowXY.getY()][nowXY.getX()].left=false;
				matrix[nowXY.getY()][nowXY.getX()].way--;
			} 
		} 
		else if( matrix[nowXY.getY()][nowXY.getX()].down) {
			nowXY.setXY( nowXY.getX(), nowXY.getY()+1) ;
			if ( matrix[nowXY.getY()][nowXY.getX()].ch == 'G' )
 				goal.setXY( nowXY.getX(), nowXY.getY() ) ;
			
			if (matrix[nowXY.getY()][nowXY.getX()].up && matrix[nowXY.getY()][nowXY.getX()].ch != 'G'  ) { 
				matrix[nowXY.getY()][nowXY.getX()].up=false;
				matrix[nowXY.getY()][nowXY.getX()].way--;
			} 
		}   
		else if( matrix[nowXY.getY()][nowXY.getX()].left) {
			nowXY.setXY( nowXY.getX()-1, nowXY.getY() ) ;
			if ( matrix[nowXY.getY()][nowXY.getX()].ch == 'G' )
 				goal.setXY( nowXY.getX(), nowXY.getY() ) ;
			
			if (matrix[nowXY.getY()][nowXY.getX()].right && matrix[nowXY.getY()][nowXY.getX()].ch != 'G' ) { 
				matrix[nowXY.getY()][nowXY.getX()].right=false;
				matrix[nowXY.getY()][nowXY.getX()].way--;
			} 
		}  
		else if( matrix[nowXY.getY()][nowXY.getX()].up ) {
	
			nowXY.setXY( nowXY.getX(), nowXY.getY()-1 ) ;
			if ( matrix[nowXY.getY()][nowXY.getX()].ch == 'G' )
 				goal.setXY( nowXY.getX(), nowXY.getY() ) ;
 				
			if ( matrix[nowXY.getY()][nowXY.getX()].down && matrix[nowXY.getY()][nowXY.getX()].ch != 'G') { 
				matrix[nowXY.getY()][nowXY.getX()].down=false;
				matrix[nowXY.getY()][nowXY.getX()].way--;
			}
		}  
		
		if( matrix[nowXY.getY()][nowXY.getX()].ch == 'E' )	// �ѹ����ʫ� �Y�{�b��m�OE �h�אּV 
			matrix[nowXY.getY()][nowXY.getX()].ch = 'V' ;
		
	}

	bool FindGoals( int goalsNum ) {
		
		bool firstgoal=true ; 
		matrixBackup() ;
		Initialize(); // �N�g�c�U�Ӧ�m���W�U���k��l�� 
		SetObstacle() ; // ��Obstacle��m 
		int x=0,y=0;
		nowXY.setXY(x, y);
					
		while( goalsNum != 0 ){
				
			while( matrix[nowXY.getY()][nowXY.getX()].ch != 'G' && matrix[nowXY.getY()][nowXY.getX()].way != 0 && visitedExistE() ) {
				// ��S���G �� �٦����i�H�� �� V�g�c�٦s�bE �N�~�� 
				NextStep();			
			}
					
			if ( matrix[nowXY.getY()][nowXY.getX()].ch == 'G' ){
				matrixToVisited(); // �N�ѹ����L���a��copy��V 
				goalsNum--; // ���ؼ� 
				changeVtoR(); // �]�����G�F �N�ѹ���訫�L��V����R 
				if ( !firstgoal ) // �p�G���O�Ĥ@���� �N�U��Q���N��G ���^�� 
					matrix[x][y].ch = 'G';
				firstgoal=false;
					
				if( goalsNum != 0 ) {
					Initialize();
					SetObstacle(); 
					x=goal.getX(); // ����G���y�� �U�����t�@��G����i�H��o��G ( �w�ܦ�V ) �A�ܦ^G 
					y=goal.getY();
					nowXY.setXY(x,y); // �NG�I�]���_�I �~���U�@��G 
					matrix[nowXY.getY()][nowXY.getX()].ch = 'V' ; // �NG�ܦ�V ���ѹ��i�H�~��] 
				} 
				else
					return true;
			} 					
			else if ( matrix[nowXY.getY()][nowXY.getX()].way == 0 || visitedExistE() ) { // ��S���i���F �άO V�g�c�S��E�F 
				matrixToVisited();
				SetDeadRoad(); 
				clr();
				nowXY.setXY(x,y) ;
			}
			else if( !visitedExistE() ) {
				return false ;	
			}
			
		}	
		
		return false;
	}
	
	bool match( int i ) { // ��n�䪺�ؼмƶq�j��g�c���ؼм� 
		
		int num=0;
		
		for( int a=0; a<coMax.getY(); a++){
			for( int b=0; b<coMax.getX(); b++){
				if ( matrix[a][b].ch == 'G' ) 
					num++;
			}
		}
		
		if ( num>=i )
			return true;
		return false;
		
	} 
	
	void changeVtoR() {
		
		for( int a=0; a<coMax.getY(); a++ ){
			for( int b=0; b<coMax.getX(); b++){
				if( matrix[a][b].ch =='V')
					matrix[a][b].ch = 'R' ;
			}
		}
	}	
	
	void ShowRPath(){
		
		for(int a=0; a<coMax.getY(); a++){
			for(int b=0; b<coMax.getX(); b++)
				cout << matrix[a][b].ch ;
			cout << endl ;
		}		
		
	}
	
	void ShowVPath(){
		
		for(int a=0; a<coMax.getY(); a++){
			for(int b=0; b<coMax.getX(); b++)
				cout << visitedMatrix[a][b].ch ;
			cout << endl ;
		}	
		
	}	
};

int main() {
	
	int command=0, numOfGoals=1;
	
	Maze nowMaze ;
	string fileName;
	
	cout << endl << "0.Quit" << endl << "1.One Goal" << endl << "2.More Goals" << endl << "****************" << endl ;
	cout << "Please input a command( 0 or 1 or 2 ) :" << endl;
	cin >> command ;
	
	if (command==0) // ��0���}�{�� 
		return 0;
	
	if ( command!=1 && command!=2 ) {
		cout << "Error!  �п�J���T�����O " << endl;
		return main(); 
	}

	if(command==1 || command==2){ // �P�_�n�ϥΥ���1�٬O����2 

		cout << "Please input a file number( Ex. 201 . 202 . 203 ...so forth ) " << endl ;
		cin >> fileName ;  // ��J�ɦW 
				
		if ( nowMaze.LoadFile( fileName ) ) {  // ��Ƨ��������ɮ� 
			
			if( command==2 ) {
				cout << "�п�J�ؼЪ��ӼƢ�=? ( N >= 1 )" << endl ;
				cin >> numOfGoals ;
				if( !nowMaze.match( numOfGoals ) ) {
					cout << "Error : Goals�Ƥp��N! " << endl;
					return main() ;
				}
				if( numOfGoals<1 ) {
					cout <<  " Error! N���ର�t�� " << endl;
					return main() ;
				}
			}
	
			if ( nowMaze.FindGoals( numOfGoals ) ) {
				cout<< endl;
				nowMaze.ShowVPath() ;
				cout << endl;
				nowMaze.ShowRPath() ;
			}
			else {
				cout << "���s�b���|" << endl;
				return main() ; 
			}
		}
		else
			cout << "���Ū�����~" << endl ; 

	}
	return main();
	
}

