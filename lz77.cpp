/**********************************************
 *           ___ALGORITHM LZ77___             *
 *                                            *
 *    GitHub: https://github.com/OrionBelt3   *  
**********************************************/


/*___________________________________________________________
I variant:                                                   |
    input: </name_program> <compress/decompress> <file_name> |
    output: compress -> out_encoded.txt                      |
            decompress -> out_decoded.txt                    |
                                                             |
II variant:                                                  |
    input: </name_program>                                   |
        (std::cin)-> compress/decompress                     |
        (std::cin)-> text for encode/ text for decode        |
    output:                                                  |
        (std::cout)-> encoded text/ decoded text             |
____________________________________________________________*/
#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>

//size buffer
static int BUFFER_SIZE = 5;

//<offset,length, next>
struct Node3
{
    int offset = 0;
    int length = 0;
    char next;
};

/**********************
 * block of auxiliary * 
 * functions          *
 **********************/

/*
findMatching - searches for a substring
     that can be found in the buffer or built from the buffer

shiftBuffer - moves the buffer
     depending on the substring found earlier

encodedInString - turns a vector into a string consisting
     of triples of numbers

stringInDecoded - the inverse function
     of this function: encodedInString
*/

//------------------------------------------------------------------------//
Node3 findMatching(std::string buffer, std::string subString, int pos){
    bool flagOpen = false;
    int p, k;
    Node3 node;
    std::string sub = "";
    for(int i = 0; i < buffer.size()+1; ++i){ 
        sub.push_back(subString[pos+i]);
        p = buffer.rfind(sub);
        if(i == buffer.size() -1 && p >= 0  && subString[pos] == subString[pos+buffer.size()]){
            flagOpen = true;
            break;
        }
        if(p >= 0){
            node.offset = buffer.size() - p;
            node.length = i+1;
        } else {
            node.next = subString[pos+i]; 
            break;
        }
    }

    if(flagOpen){
        k = pos - p;
        node.offset = pos - p;
        while(subString[pos] == subString[pos+buffer.size()]){
            sub.push_back(subString[pos+buffer.size()]);
            ++pos;
            ++k;
        }
        node.length = k;
        node.next = subString[pos+buffer.size()];
    }
    
    return node;
}

std:: string shiftBuffer(int pos, std::string s){
    std::string buffer;
    if(pos < BUFFER_SIZE){
        buffer.append(s, 0, pos);
    } else {
         buffer.append(s, pos-BUFFER_SIZE, BUFFER_SIZE); 
    }
    return buffer;
}

std::string encodedInString(std::vector<Node3> compress){
    std::string stringEncoded = "";
        for(int i = 0; i < compress.size() - 1; ++i){
        stringEncoded.push_back('<');
        stringEncoded.append(std::to_string(compress[i].offset));
        stringEncoded.push_back(',');
        stringEncoded.append(std::to_string(compress[i].length));
        stringEncoded.push_back(',');
        stringEncoded.push_back(compress[i].next);
        stringEncoded += '>';
        stringEncoded += ',';
        //stringEncoded += ' ';
    }
    stringEncoded.push_back('<');
        stringEncoded.append(std::to_string(compress[compress.size() - 1].offset));
        stringEncoded.push_back(',');
        stringEncoded.append(std::to_string(compress[compress.size() - 1].length));
        stringEncoded.push_back(',');
        stringEncoded.push_back(compress[compress.size() - 1].next);
        //stringEncoded.push_back(EOF);
        stringEncoded += '>';
    return stringEncoded;
}

std::vector<Node3> stringInDecoded(std::string stringEncoded){
    std::vector<Node3> compress;
    std::string stringNumber = "";
    int k, p;

    Node3 node;
    for(int i = 0; i < stringEncoded.size(); ++i){
        if(stringEncoded[i] == '<'){
            (++i);
            while(stringEncoded[i] != ','){
                stringNumber.push_back(stringEncoded[i]);
                (++i);
            }
            node.offset = stoi(stringNumber);
            stringNumber = "";

            (++i);
            while(stringEncoded[i] != ','){
                stringNumber.push_back(stringEncoded[i]);
                (++i);
            }
            node.length = stoi(stringNumber);
            stringNumber = "";

            (++i);
            if(stringEncoded[i] != '>'){
                node.next = stringEncoded[i];
            }else {
                node.next = EOF;
            }
                
        }
        if(stringEncoded[i] == '>'){
            compress.push_back(node);
        }
        
    }
    return compress;
}
//------------------------------------------------------------------------//


/**********************
 * block of main      * 
 * functions          *
 **********************/
/*
encoderLZ77 - encodes text in the form of
     triples of numbers

decodeLZ77 - decodes triples of numbers into plain text
*/
//------------------------------------------------------------------------//
std::vector<Node3> encoderLZ77(std::string s){
    std::string buffer;
    std::string subString;
    std::vector<Node3> answer;
    Node3 node;

    if(s.length() < BUFFER_SIZE){
        for(int i = 0; i < s.length(); ++i){
            node.next = s[i];
            answer.push_back(node); 
        }
        return answer;
    } 

    int pos = 0;
    buffer = "";
    while (pos < s.length()){
        //buffer = createBuffer(s, pos, node.length + 1); 
        node = findMatching(buffer, s, pos); 
        //node.next = s[pos]; 
        answer.push_back(node);
        pos += node.length + 1;
        buffer = shiftBuffer(pos, s);
    }
    return answer;
}

std::string decodeLZ77(std::vector<Node3> encoded){
    std::string answer = "";
    int start;
    for(int i = 0; i < encoded.size(); ++i){
        if(encoded[i].length > 0){
            start = answer.size() - encoded[i].offset;
            for(int j = 0; j < encoded[i].length; ++j){
                answer.push_back(answer[start+j]);
            }
        }
        answer.push_back( encoded[i].next);
    }
    return answer;
}
//------------------------------------------------------------------------//

int main(int argc, char *argv[]){
    std::string operation;
    std::string inputFile;
    std::string outputFile = "out.txt";
    std::string text;
    std::string encodedText;
    std::string textLine;
    std::string encodedLine = "";
    std::string decodedLine = "";
    std::vector<Node3> lineCompress;
    std::vector<Node3> lineDecompress;
    
    if(argc > 1 && (argc <= 2 || argc > 3)){
        
        std::cout << "Usage: <name program> <compress/decompress> <file name>" << std::endl;
        exit(1); 
    } else if (argc > 1) {
        inputFile = argv[2];
        if(strcmp(argv[1], "compress") == 0){
            std::ifstream in(inputFile);
            std::ofstream out;
            out.open("out_encoded.txt");
            if(in.is_open() && out.is_open()){
                while(getline(in, textLine)){ 
                    lineCompress = encoderLZ77(textLine);
                    encodedLine = encodedInString(lineCompress);
                   // std::cout <<  encodedLine << std::endl;
                    out << encodedLine << std::endl;
                    encodedLine = "";
                }
            }
            in.close();
            out.close();

        } else if(strcmp(argv[1], "decompress") == 0){
            std::ifstream in(inputFile);
            std::ofstream out;
            out.open("out_decoded.txt");
            if(in.is_open() && out.is_open()){
                while(getline(in, textLine)){ 
                    lineDecompress = stringInDecoded(textLine);
                    decodedLine = decodeLZ77(lineDecompress);
                    decodedLine.pop_back(); //removes the invisible EOF character
                    //std::cout <<  decodedLine << std::endl;
                    out << decodedLine << std::endl;
                    decodedLine = "";
                }
            }
            in.close();
            out.close();
        } else {
            std::cout << "Usage: <name program> <compress/decompress> <file name>" << std::endl;
            exit(1);
        }
    } else {
       
        std::cin >> operation;
        if(operation == "compress"){
            std::cin >> text;
            lineCompress = encoderLZ77(text);
            encodedLine = encodedInString(lineCompress);
            std::cout <<  encodedLine << std::endl;
        } else if (operation == "decompress"){
            std::cin >> encodedText;
            lineDecompress = stringInDecoded(encodedText);
            decodedLine = decodeLZ77(lineDecompress);
            //decodedLine.pop_back();
            std::cout <<  decodedLine << std::endl;
        } else {
            std::cout << "ERROR: waring flag!" << std::endl;
            exit(1);
        }  
    }
}
