// DS1#2_06_10727108許瀚文_10727125石慕評 

#include<iostream>
#include<string>
#include<cstdio>

using namespace std;

typedef enum { Copy=-1, Right,Down,Left,Up} Moves;
// 表示每一步的方向 

struct Judge {
	bool up;
	bool down;
	bool left;
	bool right;
	char ch;
	int way;
};

class CoXY{
	
	int x; // X軸 
	int y; // Y軸 
	
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
	
	int maxX, maxY;			// 用來讀迷宮的長寬 
	CoXY coMax;				// 其XY存放迷宮的長度 
	CoXY nowXY;				// 老鼠現在位置 
	CoXY branch;			// 老鼠遇到岔路時  紀錄座標位置 
	CoXY goal;				// 當老鼠要找的目標大於一個時  先找到的那個紀錄座標 
	Judge **matrix; 		// 存放R之迷宮 
	Judge **visitedMatrix;  // 存放V之迷宮 
	
	void matrixBackup() {	// 當V迷宮空著時  將R迷宮之資訊複製到V上 
		for( int a=0; a<coMax.getY(); a++ ){
			for( int b=0; b<coMax.getX(); b++){
				visitedMatrix[a][b].ch = matrix[a][b].ch ;
			}
		}
	}
	
	bool visitedExistE() {  // 判斷V迷宮是否還有E存在  沒有表示走完了卻沒找到目標 
		
		for( int a=0; a<coMax.getY(); a++ ){
			for( int b=0; b<coMax.getX(); b++){
				if( visitedMatrix[a][b].ch =='E')
					return true ;
			}
		}
		
		return false ;
		
	} 
	
	void clr() {  // 將迷宮初始化 
		
		for( int a=0; a<coMax.getY(); a++ ){
			for( int b=0; b<coMax.getX(); b++){
				if ( matrix[a][b].ch == 'V' )
					matrix[a][b].ch = 'E' ;
			}
		}
		
	}
	
	void matrixToVisited() { // 將老鼠所走之路copy至V迷宮 
	
		for( int a=0; a<coMax.getY(); a++) {
			for( int b=0; b<coMax.getX(); b++){
				if( matrix[a][b].ch == 'V' ) 
					visitedMatrix[a][b].ch = 'V' ;
			}
		}	
	
	}
	
	void Initialize(){ // 初始化每一格位置之上下左右 
		
		for( int a=0; a<maxY; a++ ){
			for( int b=0; b<maxX; b++){
				matrix[a][b].up = true ;    //往上走 
				matrix[a][b].down = true ;  //往下走 
				matrix[a][b].right = true ; //往右走 
				matrix[a][b].left = true ;  //往左走走 
				matrix[a][b].way = 4;       //每一格有4條岔路 
				
			}
		}
		
	}	
	
	bool LoadFile( string fileName ) {      // 讀檔案 
	
		FILE *infile = NULL ;
		bool success = false ;
		char temp='\0', ch ;
	
		fileName = "input" + fileName + ".txt" ;
		infile = fopen( fileName.c_str(), "r") ;
	
		if ( infile == NULL )                   // 判斷是有此檔名 
			cout << fileName << "does not exist!" << endl ;
		else {
			
			fscanf( infile, "%d %d", &maxX, &maxY ) ; // 讀入檔案中的數字設定迷宮的長與寬 
			fscanf( infile, "%c", &ch ) ;             // 讀入檔案中的迷宮 
			coMax.setXY( maxX, maxY ) ;				  // 設定迷宮之長寬 
			matrix = new Judge*[ coMax.getY()+1];	  // 設置動態陣列  Y軸 
			visitedMatrix = new Judge*[ coMax.getY()+1];
			
			for( int a=0; a<coMax.getY(); a++) {	  // 設置動態陣列 X軸 
				matrix[a] = new Judge[ coMax.getX()] ;
				visitedMatrix[a] = new Judge[ coMax.getX()] ;
			}
		
			if ( coMax.getX() > 0 && coMax.getY() > 0 ) {
				int nowLine=0;						  // 目前Y軸 

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
	
	void SetObstacle() {  // 設定障礙 
		
		
		for( int a=0; a<coMax.getY(); a++ ){
			for( int b=0; b<coMax.getX(); b++ ){

				if( a==0 ){                // 迷宮上方邊界無法往上走 
					matrix[a][b].up=false;
					matrix[a][b].way--;
				}
					
				if( b==0 ){                 // 迷宮左方邊界無法往左走  
			        matrix[a][b].left=false;
			        matrix[a][b].way--;
			    }
					
				if( a==coMax.getY()-1 ){     // 迷宮下方邊界邊界無法往下走  
					matrix[a][b].down=false;
					matrix[a][b].way--;
			    }
			    
				if( b==coMax.getX()-1 ) {     // 迷宮右方邊界邊界無法往右走  
					matrix[a][b].right=false;
					matrix[a][b].way--;
				}
				
				if( matrix[a][b].ch =='O' ) {    //遇到障礙物 
					if( a==0 && b==0 ){          //迷宮左上角無法走往左,上方走 
						matrix[a+1][b].up=false ;
						matrix[a+1][b].way--; 
						matrix[a][b+1].left=false ;
						matrix[a][b+1].way--;						
					}
					else if( a==0 && b==coMax.getX()-1 ) {  //迷宮右上角無法走往右,上方走 
						matrix[a][b-1].right=false;
						matrix[a][b-1].way--;
						matrix[a+1][b].up=false;	
						matrix[a+1][b].way--;
					}
					else if( a==coMax.getY()-1 && b==coMax.getX()-1 ) {  //迷宮右下角無法走往右,下方走 
						matrix[a-1][b].down=false;
						matrix[a-1][b].way--;
						matrix[a][b-1].right=false;
						matrix[a][b-1].way--;
					}
					else if( a==coMax.getY()-1 && b==0 ) {  //迷宮左下角無法走往左,下方走 
						matrix[a-1][b].down=false;
						matrix[a-1][b].way--;
						matrix[a][b+1].left=false;
						matrix[a][b+1].way--;
					}
					else if ( a==0 ) {  // 當Y軸為0 表示為迷宮上緣 其上方無路可走 
						matrix[a+1][b].up=false ;
						matrix[a+1][b].way--;
						matrix[a][b+1].left=false ;
						matrix[a][b+1].way--;	
						matrix[a][b-1].right=false;
						matrix[a][b-1].way--;
					}
					else if ( b==0 ) { // 當X軸為0 表示左邊為迷宮邊緣 其左方無路可走 
						matrix[a+1][b].up=false ;
						matrix[a+1][b].way--; 
						matrix[a][b+1].left=false ;
						matrix[a][b+1].way--;
						matrix[a-1][b].down=false;
						matrix[a-1][b].way--;
					}
					else if ( a==coMax.getY()-1 ) { // 當Y軸為迷宮之長 表示下方為迷宮下緣 其下方無路可走 
						matrix[a-1][b].down=false;
						matrix[a-1][b].way--;
						matrix[a][b+1].left=false;
						matrix[a][b+1].way--;
						matrix[a][b-1].right=false;
						matrix[a][b-1].way--;
					}
					else if ( b==coMax.getX()-1) { // 當X軸為迷宮之寬 表示右方為迷宮邊緣 其右方無路可走 
						matrix[a-1][b].down=false;
						matrix[a-1][b].way--;
						matrix[a][b-1].right=false;
						matrix[a][b-1].way--;
						matrix[a+1][b].up=false ;
						matrix[a+1][b].way--;
					}
					else { // 若目前格子不為迷宮邊緣 表示其為迷宮內部 
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

	void SetDeadRoad() {  // 遇到岔路時 走錯條 將那條設為死路 
		
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

	void NextStep() {														// 老鼠走的順序為 右下左上 
		
		if ( nowXY.getX()==0 && nowXY.getY()==0 && matrix[0][0].ch == 'E' ) // 老鼠起始點為V 
			matrix[nowXY.getY()][nowXY.getX()].ch = 'V' ;
		
		if ( matrix[nowXY.getY()][nowXY.getX()].way!=1 )                    // 當老鼠判定可走的方向不只1條標示為岔路 
			branch.setXY( nowXY.getX(), nowXY.getY() ) ; 
		
		if( matrix[nowXY.getY()][nowXY.getX()].right ) {					// 若老鼠可往右走 
 			nowXY.setXY( nowXY.getX()+1, nowXY.getY() ) ;					// 移動後改變其座標 
 			if ( matrix[nowXY.getY()][nowXY.getX()].ch == 'G' )				// 若遇到Goal 將其座標記下 
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
		
		if( matrix[nowXY.getY()][nowXY.getX()].ch == 'E' )	// 老鼠移動後 若現在位置是E 則改為V 
			matrix[nowXY.getY()][nowXY.getX()].ch = 'V' ;
		
	}

	bool FindGoals( int goalsNum ) {
		
		bool firstgoal=true ; 
		matrixBackup() ;
		Initialize(); // 將迷宮各個位置之上下左右初始化 
		SetObstacle() ; // 找Obstacle位置 
		int x=0,y=0;
		nowXY.setXY(x, y);
					
		while( goalsNum != 0 ){
				
			while( matrix[nowXY.getY()][nowXY.getX()].ch != 'G' && matrix[nowXY.getY()][nowXY.getX()].way != 0 && visitedExistE() ) {
				// 當沒找到G 或 還有路可以走 或 V迷宮還存在E 就繼續走 
				NextStep();			
			}
					
			if ( matrix[nowXY.getY()][nowXY.getX()].ch == 'G' ){
				matrixToVisited(); // 將老鼠走過的地方copy給V 
				goalsNum--; // 找到目標 
				changeVtoR(); // 因為找到G了 將老鼠剛剛走過的V換為R 
				if ( !firstgoal ) // 如果不是第一次找 將下方被取代的G 換回來 
					matrix[x][y].ch = 'G';
				firstgoal=false;
					
				if( goalsNum != 0 ) {
					Initialize();
					SetObstacle(); 
					x=goal.getX(); // 紀錄G的座標 下次找到另一個G之後可以把這個G ( 已變成V ) 再變回G 
					y=goal.getY();
					nowXY.setXY(x,y); // 將G點設為起點 繼續找下一個G 
					matrix[nowXY.getY()][nowXY.getX()].ch = 'V' ; // 將G變成V 讓老鼠可以繼續跑 
				} 
				else
					return true;
			} 					
			else if ( matrix[nowXY.getY()][nowXY.getX()].way == 0 || visitedExistE() ) { // 當沒路可走了 或是 V迷宮沒有E了 
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
	
	bool match( int i ) { // 當要找的目標數量大於迷宮內目標數 
		
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
	
	if (command==0) // 按0離開程式 
		return 0;
	
	if ( command!=1 && command!=2 ) {
		cout << "Error!  請輸入正確的指令 " << endl;
		return main(); 
	}

	if(command==1 || command==2){ // 判斷要使用任務1還是任務2 

		cout << "Please input a file number( Ex. 201 . 202 . 203 ...so forth ) " << endl ;
		cin >> fileName ;  // 輸入檔名 
				
		if ( nowMaze.LoadFile( fileName ) ) {  // 資料夾中有此檔案 
			
			if( command==2 ) {
				cout << "請輸入目標的個數Ｎ=? ( N >= 1 )" << endl ;
				cin >> numOfGoals ;
				if( !nowMaze.match( numOfGoals ) ) {
					cout << "Error : Goals數小於N! " << endl;
					return main() ;
				}
				if( numOfGoals<1 ) {
					cout <<  " Error! N不能為負數 " << endl;
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
				cout << "不存在路徑" << endl;
				return main() ; 
			}
		}
		else
			cout << "資料讀取錯誤" << endl ; 

	}
	return main();
	
}

