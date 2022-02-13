#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

/* function that calculates log of specified base */
long log_func(long a, long base);

/* function that performs modular exponentiation where b=base,
 *a = power in binary and m = mod value */
long modular_exponentiation(long b, long e, string k, long m);

/* function that finds the gcd of any given numbers a and b */
long euclidean_alg(long a, long b);

/* function with extended euclidean algorithm */
long extended_euclidean_alg(long e, long L, long p, long q);

/* function that calculates the value of decryption key, d */
long decryption_key(long p, long q, long e);

/* function that converts a decimal number to binary and
and returns an array of binary values */
string decimal_to_binary(int d);

int main(int argc, char* argv[]) {
    string command;  // command type
    string input, output, filename, message, buffer;
    long n;
    long p = stol(argv[1]);
    long q = stol(argv[2]);
    long e = 65537;
    string a("10000000000000001");

    // calculates the decryption key
    long d = decryption_key(p, q, e);
    if (d == -1) {
        return -1;
    }

    string d_bi = decimal_to_binary(d);  // converts to binary

    cout << "Enter a command: ";

    while (cin >> command) {
        if (command == "EXIT") {
            return 0;  // exit the program
        } else if (command == "ENCRYPT") {
            // take the necessary inputs
            string first;
            cin >> filename >> first;
            getline(cin, buffer);
            message = first;
            message = message + buffer;

            ofstream ofile;  // output file
            ofile.open(filename);
            if (ofile.fail()) {
                cout << "Unable to open input file." << endl;
                return -1;
            }

            /* Encrypt the message */
            n = p*q;
            if (n < 27)  // if n is less than 27
            {
                cout << "Bad choice of n" << endl;
                return -1;
            }

            int x = 1 + log_func(n / 27, 100);
            long M, C;

            // loop through x letters in message
            for (unsigned int i = 0; i < message.size(); i += x) {
                string M_str;
                // loop through every letter in x
                for (unsigned int j = i; j < x + i; j++) {
                    // if message ended, add 00s
                    if (j >= message.size()) {
                        M_str = M_str + '0' + '0';
                    }
                    // if white space, add 00
                    else if (message[j] == ' ') {
                        M_str = M_str + '0' + '0';
                    }
                    // if lowercase alphabet, add the number
                    else if (message[j] > 96 && message[j] < 123) {
                        int num = message[j] - 96;
                        string temp = to_string(num);
                        if (num < 10) {
                            M_str = M_str + '0' + temp;
                        } else {
                            M_str = M_str + temp;
                        }
                    }
                }
                M = stoi(M_str);                         // convert to an actual int
                C = modular_exponentiation(M, e, a, n);  // do modular exp
                ofile << C << " ";                       // write C to the output file
            }
        } else if (command == "DECRYPT") {
            // take in the rest of the arguments
            cin >> input >> output;

            ifstream ifile;  // input file
            ifile.open(input);
            if (ifile.fail()) {
                cout << "Unable to open input file." << endl;
                return -1;
            }

            ofstream ofile;  // output file
            ofile.open(output);

            /* Decrypt the message */

            n = p * q;  // calculate n
            int x = 1 + log_func(n / 27, 100);
            long C, M;
            string M_str;
            string message;  // string for the message

            // while there are numbers in the file
            while (ifile >> C) {
                M = modular_exponentiation(C, d, d_bi, n);  // find M
                M_str = to_string(M);                       // convert M to string

                // if M's size is not equal to 2x, add the necessary 0s
                if (M_str.size() < 2 * x) {
                    while (M_str.size() != 2 * x) {
                        M_str.insert(M_str.begin(), '0');
                    }
                } else if (M_str.size() > 2 * x) {
                    cout << "Bad p and q numbers." << endl;
                    return -1;
                }

                // loop though M by 2
                for (unsigned int i = 0; i < M_str.size(); i += 2) {
                    string temp;                            // char with 2 digits
                    temp = temp + M_str[i] + M_str[i + 1];  // set temp to M's digits
                    char letter;                            // corresponding letter

                    // if temp is 00, then white space
                    if (temp[0] == '0' && temp[1] == '0') {
                        letter = ' ';
                    } else {
                        // if the 1st digit is 0
                        if (temp[0] == '0') { 
                            letter = temp[1] + 48;
                        } else {
                            int num = stoi(temp);
                            letter = num + 96;
                        }
                    }

                    message = message + letter;
                }
            }

            ofile << message;  // outputs the decrypted message

            ifile.close();
            ofile.close();
        } else {
            cin.clear();  // clear the input stream
            cout << "Invalid command. Enter a valid command." << endl;
        }

        cout << "Enter a command: ";  // enter a new command
    }
}

/* other function implementations */

long log_func(long a, long base) { return log10(a) / log10(base); }

long modular_exponentiation(long b, long e, string k, long m) {

    // int array_size = log_func(e, 2) + 1;
    long x = 1;
    long power = b % m;

    for (unsigned int i = 0; i < k.size(); i++) {
        if (k[i] == '1') {
            x = (x * power) % m;
        }
        power = (power * power) % m;
    }
    return x;
}

long euclidean_alg(long a, long b) {
    long x, y, r;
    x = a;
    y = b;

    while (y != 0) {
        r = x % y;
        x = y;
        y = r;
    }

    return x;
}

long extended_euclidean_alg(long e, long L, long p, long q) {
    long s = 0, t = 1, r = e;
    long old_s = 1, old_t = 0, old_r = L;

    while (r != 0) {
        long quotient = old_r / r;
        long temp = r;
        r = old_r - quotient * r;
        old_r = temp;
        temp = s;
        s = old_s - quotient * s;
        old_s = temp;
        temp = t;
        t = old_t - quotient * t;
        old_t = temp;
    }

    if (old_r != 1) {
        cout << "Bad choice of p and q" << endl;
        return -1;
    }

    // if t is negative, add (p-1)*(q-1) to it
    if (old_t < 0) {
        old_t += (p - 1) * (q - 1);
    }

    return old_t;
}

long decryption_key(long p, long q, long e) {
    long gcd_p_q;
    if (p > q) {
        gcd_p_q = euclidean_alg(p - 1, q - 1);
    } else {
        gcd_p_q = euclidean_alg(q - 1, p - 1);
    }
    long L = ((p - 1) * (q - 1)) / gcd_p_q;  // compute lcm
    if (L <= e) {
        cout << "Bad p and q numbers" << endl;
        return -1;
    }

    long d = extended_euclidean_alg(e, L, p, q);

    return d;
}

string decimal_to_binary(int d) {
    int array_size = log_func(d, 2) + 1;  // size of future string
    string d_bi;
    for (int i = 0; i < array_size; i++) {
        char temp = (d % 2) + 48;
        d_bi = d_bi + temp;
        d /= 2;
    }
    return d_bi;
}