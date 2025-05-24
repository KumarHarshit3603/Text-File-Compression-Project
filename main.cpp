#include <iostream>
#include <queue>
#include <map>
#include <bitset>
#include <fstream>
class TreeNode{
    public:
    char character;
    int value;
    TreeNode* left;
    TreeNode* right;
    TreeNode(char character,int value){
        this->character=character;
        this->value=value;
        left=nullptr;
        right=nullptr;
    }
    TreeNode(int a,int b,TreeNode* left, TreeNode* right){
        this->value= a+b;
        this->left=left;
        this->right=right;
        this->character=-1;
    }
};
class compare{
    public:
    bool operator()(TreeNode* a, TreeNode* b){
        if(a->value==b->value) return a->character>b->character;
        else return a->value>b->value;
    }//comparator for minheap used
};
TreeNode* huffman_tree(std::map<char,int>mpp){
    std::priority_queue<TreeNode*,std::vector<TreeNode*>,compare> minheap;
    for(auto i:mpp){
        TreeNode* node = new TreeNode(i.first,i.second);
        minheap.push(node);
    }
  
    while(minheap.size()!=1){
        TreeNode* a=minheap.top();
        minheap.pop();
        TreeNode* b=minheap.top();
        minheap.pop();
        TreeNode* combination= new TreeNode(a->value,b->value,a,b);
        minheap.push(combination);
    }
    return minheap.top();

}
std::streamsize write_to_file_compression(std::string binary_string,std::map<char,int> mpp){
    std::fstream fout;
    std::cout<<"what name you want to give to the file?";
    std::string name;
    getline(std::cin,name);
    fout.open(name+".bin",std::ios::out | std::ios::binary );
    if(fout){
            int map_size=mpp.size();//number of key value pairs
             int x=8-binary_string.length()%8;
            fout.write(reinterpret_cast<char*>(&map_size),sizeof(int));
            fout.write(reinterpret_cast<char*>(&x),sizeof(int));
            for(auto it:mpp){
                fout.write(&it.first,sizeof(it.first));
                fout.write(reinterpret_cast<char*>(&it.second),sizeof(int));
            }
            int j=0;
            while(j<binary_string.length()){
                std::bitset<8> a=0;
                for(int i=j;i<j+8&&i<binary_string.length();i++){
                    a= a<<1 | (std::bitset<8>)(binary_string[i]-'0');
                }
                j+=8;
                if(j>binary_string.length()){
               
                a=a<<x;
                }
                fout.write(reinterpret_cast<char*>(&a),1);
            }
            
            std::cout<<"file has been successfully created."<<std::endl;
            
    }
    else{
        std::cerr<<"error";
    }
            fout.seekg(0,std::ios::end);
            std::streamsize size=fout.tellg();
            fout.close();
            return size;
}
void encoder(TreeNode* root,std::string s,std::map<char,std::string> &m){
    if(root->left==nullptr&&root->right==nullptr){
        m[root->character]=s;
        return;
    }
    encoder(root->left,s+'0',m);
    encoder(root->right,s+'1',m);

}
void compression(){
    std::string s;
    std::string filename;
    std::cout<<"please enter the filename or path:"<<std::endl;
    std::cin.ignore();
    std::getline(std::cin,filename);
    
    std::fstream fin;
    fin.open(filename,std::ios::in);
    std::string line;
    while(getline(fin,line)){
        line+='\n';
       s+=line;
    }
    fin.close();
    std::map<char,int> mpp;

    for(int i=0;i<s.length();i++){
        mpp[s[i]]++;
    }
    TreeNode* root = huffman_tree(mpp);
    std::string str="";
    std::map<char,std::string> m;
       encoder(root,str,m);
    for(int i=0;i<s.length();i++){
        str+=m[s[i]];
    }
   std::streamsize size= write_to_file_compression(str,mpp);
    std::cout<<"original size: "<<s.length()<<" bytes"<<std::endl;
    std::cout<<"compressed size: "<<size<<" bytes"<<std::endl;
    float ratio = (size*100)/s.length();
    std::cout<<"compression ratio: "<<ratio<<"%"<<std::endl;
    
}
std::string decoder(TreeNode* root,std::string code_string){
        int i=0;
        std::string text="";
        TreeNode* rootcopy=root;
        while(i<code_string.length()){
            if(code_string[i]=='0'){
                root=root->left;
            }
            else if(code_string[i]=='1'){
                root=root->right;
            }
            if(root->left==nullptr&&root->right==nullptr){
                text+=root->character;
                root=rootcopy;
            }
            i++;
        }    
        return text;
}
void decompression(){
    std::string s;
    std::map<char,int> freq_table;
    std::fstream fin;
    std::cout<<"enter the file name or path"<<std::endl;
    std::string filename;
    std::cin.ignore();
    getline(std::cin,filename);
    fin.open(filename,std::ios::in | std::ios::binary);
    if(!fin){
        std::cerr<<"error opening file"<<std::endl;
    }
    else{
        int freq_table_size;
        fin.read(reinterpret_cast<char*>(&freq_table_size),sizeof(int));
        int x;
        fin.read(reinterpret_cast<char*>(&x),sizeof(int));
        while(freq_table_size--){
            char a;
            int n;
            fin.read(reinterpret_cast<char*>(&a),sizeof(char));
            fin.read(reinterpret_cast<char*>(&n),sizeof(int));
            freq_table[a]=n;
        }
        uint8_t code;
        
        while(fin.read(reinterpret_cast<char*>(&code),1)){
                std::string bytestring="";
                if(code==0) bytestring+="00000000";
                while(code>0){
                    bytestring= (code%2==0 ? '0':'1') + bytestring;
                    code= code/2;
                }
                while(bytestring.length()<8){
                    bytestring='0'+bytestring;
                }
                s+=bytestring;
        }
        fin.close();
        while(x--){
            s.pop_back();
        }

    }
    TreeNode* root=huffman_tree(freq_table);
    std::string text=decoder(root,s); 
    std::fstream fout;
    std::cout<<"name of the output file:"<<std::endl;
    std::string output_file_name;
    getline(std::cin,output_file_name);
    fout.open(output_file_name+".txt",std::ios::out);
    if(fout){
         fout<<text;
    }
    else {
        std::cerr<<"error opening file";
    }
    fout.close();
    
    std::fstream check;
    check.open(output_file_name +".txt",std::ios::in);
    if(check){
        std::cout<<"extraction successfull"<<std::endl;
    }
    else{
        std::cout<<"error"<<std::endl;
    }
    check.close();
    
}
int main(){
     char option='y';
     while(option=='y'){
        std::cout<<"what do you want to do?"<<std::endl;
        std::cout<<"1.compress.\n2.decompress\nenter 1 or 2"<<std::endl;
        int a;
        std::cin>>a;
        if(a==1) compression();
        else if(a==2) decompression();
        else std::cout<<"invalid input"<<std::endl;
        std::cout<<"continue program(enter y)"<<std::endl;
        std::cout<<"exit program?(enter n)"<<std::endl;
        std::cin>>option;
        
     }
     
}