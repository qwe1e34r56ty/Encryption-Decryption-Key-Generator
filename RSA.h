#ifndef _RSA_H_
#define _RSA_H_

// p�� q�� �̿��� n, d, e ����

class RSA {
private:
    // ���� ����
    RSA() {};
    static long long findE(long long n) {
        for (long long i = 2; i < n; i++) {
            if (gcd(n, i) == 1) {
                return i;
            }
        }
        return -1;
    }

    static long long findD(long long e, long long n) {
        long long d = 1;
        while (true) {
            if (((d * e) % n) == 1) {
                return d;
            }
            d++;
        }
    }

    static long long gcd(long long a, long long b) {
        return (a == 0) ? b : gcd(b, a % b);
    }
public:
    static void rsa(long& p, long& q, long& n, long& d, long& e) {
        n = p * q;
        e = findE((p - 1) * (q - 1));
        d = findD(e, (p - 1) * (q - 1));
    }
};

#endif
