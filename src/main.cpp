#include <iostream>

#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include "message.h"
#include "key.h"
#include "language_data.h"
#include "solver.h"

using namespace std;

int main(int argc, char **argv) {
	if (argc != 2)
	{
		cout << "Input file name required!" << endl;
		return 1;
	}
	std::ifstream t(argv[1]);
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::string str = buffer.str();
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	std::cout << str << std::endl;
	std::cout << str.size() << std::endl;
	
	LanguageData::Initialize();
	
	Message* msg = new Message(str);
	Key key;
	key.Init(msg);
	cout << LanguageData::GetLetterFrequency('A') << endl;
	/*cout << "WLE (59): " << LanguageData::GetTrigramFrequency("WLE") << endl;
	cout << "ATOK (13): " << LanguageData::GetTetragramFrequency("ATOK") << endl;
	cout << "CIBLE (29): " << LanguageData::GetPentagramFrequency("CIBLE") << endl;*/

	std::string testmsg = "ILIKEKIGLINDPEOPGEHECAUSEITISSOMUCHFUNITIDMOREBUTTHANKINGINDWILLDAMEINTHEFORRESTHECAUSEMATISTHEMOATCANDERTUEANAMAGOBANGTOKINGSOMETHINDDIVESMETHEMOATTHRINGITDEUPERENCEITISEVENHETTERTHANDETTITDYOURROCKSOFBWITHADIRLTHEHESTPARTOFITIATHAEWHENICIEIWINGHEREHORNINPARALICEDTCALLTHEIHAVEKILLELWINGHECOMEMYSNAVESIWIGLNOTDIVEYOUMYNAMEHECAUSEYOUWILNTRYTOSGOICOWTORDTOPMYCONNECTINDOBSLAVESFORMYAFTERGIBEEHEORIETEMETHHPITI";
	std::string testmsg2 = "INADEDITNSLLFEAFTEHECAUSIITARSOMUCHNUTITSUMOAEKUBRHINDINTALLWINGLEMESTTHENORCESTHECOUREMABISTHIMAORDINLEATUEELOMATOKINTTODANTRAMETHITLLSVESMETHEMOORTHRINTABLEPFICENCEITSSEVELHETREATHETLITTIBLYOURCOCDRANKWARHOLIANTHEHESTFARTONSRIOTHEEWHENADIESWINTHECEHOALATFIREGICIUBDANNTHESHIVEDINNEGWANTHECAMEMYSNOVERIWITNLORLIVEYOUMYTIMEHECAUSEYOUWINNTAYTOSTAIDAWBOCUTOFMYCONNICRALLAKSNAVERNOAMYONTERTAKEIHEARIETIMETHHFITS";
	
	Solver* solver = new Solver(msg, key);
	key.PrintKey();
	/*solver->TestScore(testmsg);
	solver->TestScore(testmsg2);
	return 0;*/
	solver->Start();
	
    return 0;
}
