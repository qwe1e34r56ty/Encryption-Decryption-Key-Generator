#include<iostream>
#include<string>
#include<fstream>

using namespace std;

long long encoding(long long M, long long N, long long e) {
	if (e > 1) {
		return ((encoding(M, N, e / 2) % N) * (encoding(M, N, e / 2) % N) * encoding(M, N, e % 2)) % N;
	}
	else if (e == 1) {
		return M % N;
	}
	else {
		return 1 % N;
	}
}

long long decoding(long long C, long long N, long long d) {
	if (d > 1) {
		return (decoding(C, N, d / 2) * decoding(C, N, d / 2) * decoding(C, N, d % 2)) % N;
	}
	else if (d == 1) {
		return C % N;
	}
	else {
		return 1 % N;
	}
}

bool encodingFile(string in, string out, long N, long e) {
	ifstream ifs{ in };
	ofstream ofs{ out };
	char c;
	int m;
	string m_s;
	if (!ifs.is_open()) {
		cout << "파일명을 찾을 수 없습니다.";
		return false;
	}
	while (!ifs.eof()) {
		c = ifs.get();
		m = encoding(c, N, e);
		m_s = to_string(m);
		if (m == -1) {
			break;
		}
		ofs.write(m_s.c_str(), m_s.length());
		ofs.write("\n", 1);
	}
	ofs.close();
	ifs.close();
	return true;
}

bool decodingFile(string in, string out, long N, long d) {
	ifstream ifs{ in };
	ofstream ofs{ out };
	int c;
	int m_i;
	char m;
	char c_s[256];
	if (!ifs.is_open()) {
		cout << "파일명을 찾을 수 없습니다.";
		return false;
	}
	while (!ifs.eof()) {
		ifs.getline(c_s, 256);
		if (c_s[0] == '\n' || c_s[0] == 0) {
			break;
		}
		c = stoi(c_s);
		m_i = decoding(c, N, d);
		m = (char)m_i;
		ofs.put(m);
	}
	ofs.close();
	ifs.close();
	return true;
}

int main() {
	char input;
	string fileName = string();
	string ret = string();
	while (true) {
		cout << "암호화 = 1,  복호화 = 2, 그 외 종료 >> ";
		cin >> input;
		cout << "================================================" << endl;
		long N, d, e;
		switch (input) {
		case '1':
			cout << "공개키 N을 입력하세요 >> "; cin >> N;
			cout << "공개키 e를 입력하세요 >> "; cin >> e;
			cout << "암호화를 원하는 파일 명을 입력하세요 >> "; cin >> fileName;
			cout << "결과 파일 명을 입력하세요 >> "; cin >> ret;
			cout << fileName << "의 암호화 파일 생성을 시작합니다" << endl;
			if (encodingFile(fileName, ret, N, e)) {
				cout << "암호화 파일 생성을 완료했습니다" << endl;
			}
			break;
		case '2':
			cout << "개인키 N을 입력하세요 >> "; cin >> N;
			cout << "개인키 d를 입력하세요 >> "; cin >> d;
			cout << "복호화를 원하는 파일 명을 입력하세요 >> "; cin >> fileName;
			cout << "결과 파일 명을 입력하세요 >> "; cin >> ret;
			cout << fileName << "의 복호화 파일 생성을 시작합니다" << endl;
			if (decodingFile(fileName, ret, N, d)) {
				cout << "복호화 파일 생성을 완료했습니다" << endl;
			}
			break;
		}
		cout << "================================================" << endl;
		if (input != '1' && input != '2') {
			cout << "프로그램 종료";
			break;
		}
	}
	return 0;
}