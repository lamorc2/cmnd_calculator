#include <string>
#include <iostream>
#include <cctype>
#include <stdexcept>


/*

// TODO \\
Create String to Double Function that can handle float + int + Replace std::stoi with custom str-to-double
Finish this!
Done - Fix Subtraction (Idea: Pre parse, change all - to +-, and have - be ignored. Therefore, all subtraction will be adding a negative??)
Done - Fix Negative Handling and Subtraction at the same time??

Done - Add Parentheses Handling (break existing code into a function, and use parentheses to process functions inside strings independently)
Add Algebra - Idea : if alnum followed by = (e.g. x= 123), create variable in unordered map of uservariables by <string, value>. Then, if a letter is encountered later, check in uservariables

//=======\\

*/

const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string CYAN = "\033[36m";
const std::string BOLD = "\033[1m";
const char multiply = '*';
const char divide = '/';
const char add = '+';
const char subtract = '-';

/*
double stringtoNumber(std::string str){
	double output;
	if(str.find('.') != std::string::npos){

	}else{
		std::stoi(str)
	}
}
*/
std::string handleNegatives(const std::string& input){
	std::string output = input;
	int error_handle = 0;
	size_t pos = output.find("-",0);
	while(pos != std::string::npos && error_handle < 50){
		if(std::isalnum(output[pos-1])){
			output.insert(pos, "+");
		}
		pos = output.find("-",pos+1);
		error_handle++;

	}
	if(error_handle == 50){
		std::cout << RED << "Warning : Function 'handleNegatives(" << input << ")' iterated " << error_handle << " times, and then aborted." << std::endl;
	}
	return output;
}


double computeOperation(int num1, int num2, const char operation){ 
	double output = 0.0;
	switch(operation){

		case multiply:
			output = num1 * num2;
			break;

		case divide:
			if(num2 == 0){
				throw std::string("Division by Zero");
			}
			output = num1/num2;

			break;

		case add:
			output = num1 + num2;
			break;

		
		default:
			throw std::string("Unrecognized Operation");

	}
	return output;
}




std::string processFunctionString(const std::string& str, bool debug){
	// Simple cmnd line parse ( no parentheses )
			std::string error;
			char operations[] = {multiply,divide,add};
			bool newOperation = false;
			int i = 0;
			int k = 0;
			std::string current = str;
			size_t inputSize = current.size();
			std::string temp_eqn = "";
			std::string temp = "";
			int total = 0;
			int num1;
			int num2;
			double temp_out;
			while(!std::isalnum(current[inputSize - 1])){
				current.erase(inputSize-1,1);
				inputSize = current.size();
				i++;
				if(i > 50){
					std::cout << RED << "Error: End-of-Input Operator Trimming loop iterated over 50 times. Aborting Loop..." << RESET << std::endl;
					break;
				}
			}
			i = 0;
			while(!std::isalnum(current[0]) && current[0] != '-'){
				current.erase(0,1);
				inputSize = current.size();
				i++;
				if(i > 50){
					std::cout << RED << "Error: Start-of-Input Operator Trimming loop iterated over 50 times. Aborting Loop..." << RESET << std::endl;
					break;
				}
			}
			if(debug){
				std::cout << CYAN << "Refactored without redundant operators (maintain negatives before numbers): " << current << RESET << std::endl;
			}
			current = handleNegatives(current);
			if(debug){
				std::cout << CYAN << "Refactored without subtraction (maintain negatives): " << current << RESET << std::endl;
			}
			for(char operation : operations){
				
				if(debug){
					std::cout << GREEN << "Performing " << operation <<" operations." << RESET << std::endl;
				}
				newOperation = false;
				
				while(!newOperation){
					temp = "";
					temp_eqn = "";
					if(debug){
						std::cout <<  GREEN << "Performing new Operation" << RESET << "\nTemp_Eqn : " << temp_eqn << "\nCurrent: " << current << std::endl;
					}
					newOperation = true;
					i = 0;
					k = 0;
					
					inputSize = current.size();
					if(debug){
					std::cout << BLUE << "new for loop" << RESET << std::endl;
				}
					for(i=0;i<inputSize;i++){
						if(debug){
							std::cout << BLUE << "i :" << i << RESET << std::endl;						
							std::cout << "Checking " << current << " for Operations... Specifically " << operation << std::endl;
							std::cout << CYAN << "Current[i] is " << current[i] << std::endl;
					}
						if(std::isalnum(current[i]) || current[i] == '.'|| current[i] == '-'){
							temp += current[i];
							continue;
						}else{// operation detected

							if(current[i] != operation){ // wrong operation
								temp += current[i];
								temp_eqn += temp;
								if(debug){
									std::cout << RED << "Wrong Operation Detected ("<< current[i] << ") " << RESET << "\nTemp_Eqn : " << temp_eqn << "\nCurrent: " << current << std::endl;
								}
								temp = "";
								continue;
							}else{ // Right operation
								try{
								num1 = std::stoi(temp);
								if(debug){
									std::cout << "Num1 is " << num1 << std::endl;
								}
							}catch(std::invalid_argument e){
								error =  "Error: invalid_argument for num1 = std::stoi. Arg:" + temp;
								throw std::string("Calculations could not be completed, aborting." + error);
							}
								temp = "";
								for(k = i+1;k<inputSize;k++){// parse number after operation

									if(std::isalnum(current[k]) || current[k] == '.'|| current[k] == '-'){

										temp += current[k];
									}else{// current[k] is next operation
										break;
									}
								}

								try{
								num2 = std::stoi(temp);
							}catch(std::invalid_argument e){
								std::string error = "Error: invalid_argument for num2 = std::stoi. Arg:" + temp;
								throw std::string("Calculations could not be completed, aborting. "+ error );
							}// TODO : Replace with function that detects number type and converts accordingly
								temp = "";
								try{
									temp_out = computeOperation(num1,num2,operation);
									if(debug){										
										std::cout << GREEN << "Computing... " << YELLOW << num1 << operation << num2 << RESET << std::endl;
									}
								}catch(std::string error){
									std::cout << RED << "Computation Error: " << error << RESET << std::endl;
									throw std::string("Calculations could not be completed, aborting. " + error);
								}
								temp_eqn += std::to_string(temp_out);
								temp_eqn += current.substr(k, std::string::npos);
								if(debug){
									std::cout << GREEN << "Calculations Done. Output: " << std::to_string(temp_out) << "\nTemp_Eqn : " << temp_eqn << "\nCurrent: " << current << RESET << std::endl;
								}
								current = temp_eqn;
								temp_eqn = "";
								newOperation = false;
								break;




							}
						}

						
					}// end of for loop
					
					

					//TODO : Add check for last char being operator
					
				}
				temp_eqn += temp;
				if(debug){
					std::cout << "No more " << operation << " operations. Current: " << current << "Temp_Eqn: " << temp_eqn << std::endl;
				}
				newOperation = true;
			}
			if(debug){
			std::cout << CYAN << "Input: " << str << "   " << YELLOW << "Output: " << current << RESET << std::endl;
		}
			return current;

			


		}



int main(int argc,char* argv[]){
	program_start:
	std::string output;
	std::string userInput;
	bool debug = true;
	std::cout << BOLD << "Welcome! Disable Debug Prints?(Y/N)" << RESET << std::endl;
	std::cin >> userInput;
	if(userInput == "Y"){debug = false;}
	int error_handle;
	int inputSize;
	int answer;
	bool open = true;

	size_t open_parentheses;
	char operations[] = {multiply,divide,add};
	
	while(open){

		std::cout << "Input (0 to quit): " << std::endl;
		std::cin >> userInput;
		if(userInput == "0"){
			open = false;
			continue;
		}
		std::cout << GREEN << "Processing..." << RESET << std::endl;
		inputSize = userInput.size();
		std::string current = userInput;
		open_parentheses = current.find("(",0);
		if(open_parentheses != std::string::npos){
			
			std::string inParentheses;
			size_t nextOpen = current.find("(",open_parentheses + 1);
			size_t closed_parentheses; 
			while(open_parentheses != std::string::npos && error_handle	< 50){
				if(debug){
					std::cout << "Parentheses found in..." << current << std::endl;
				}
				error_handle++;
				closed_parentheses = current.find(")",open_parentheses); 
			if(closed_parentheses == std::string::npos){
				std::cout << RED << "Error: No matching closed parentheses found. " << RESET << std::endl;
				goto program_start;
			}else{// check for another open parentheses before closed // e.g 5+((2/3)*6)
				while(nextOpen != std::string::npos && nextOpen < closed_parentheses && error_handle < 50){
					error_handle++;
					open_parentheses = nextOpen;
					nextOpen = current.find("(",open_parentheses + 1);
				}
				inParentheses = current.substr(open_parentheses + 1,closed_parentheses - 1);
					try{		
							if(debug){
								std::cout << RED << "Processing equation :" << inParentheses << RESET << std::endl;
							}
							inParentheses = processFunctionString(inParentheses,debug);
					}catch(const std::string e){
							std::cout << RED << e << RESET << std::endl;
							continue;
					}
				current.replace(open_parentheses, closed_parentheses - open_parentheses + 1, inParentheses);
				if(debug){
					std::cout << BLUE << "After parentheses removal, Current: " << current << RESET << std::endl;
				}
				open_parentheses = current.find("(", 0);





			}

		}
		
		try{
			output = processFunctionString(current,debug);
			std::cout << YELLOW << "Final Output: " << GREEN << output << RESET << std::endl;
		}catch(const std::string e){
			std::cout << RED << e << RESET << std::endl;
			continue;
			}
		}else{
				try{
				output = processFunctionString(userInput,debug);
				std::cout << YELLOW << "Final Output: " << GREEN << output << RESET << std::endl;
			}catch(const std::string e){
				std::cout << RED << e << RESET << std::endl;
				continue;
			}
		}

		
	}// end of while loop


	std::cout << RED << "Closing Application..." << RESET << std::endl;
	return 0;
}