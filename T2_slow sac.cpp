#include <bits/stdc++.h>
using namespace std;
const int N = 8000;

#define byte unsigned short

byte piS[1<<4] = {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7};
byte piP[17] = {0, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16};
byte invpiS[16], invpiP[17];
byte Sboxtransfer[2][1<<16];
byte Pboxtransfer[2][1<<16];
byte K[6], L[6];

inline byte St(byte x, byte s[]) {
    return (s[(x&0xf000)>>12]<<12)|(s[(x&0x0f00)>>8]<<8)|(s[(x&0x00f0)>>4]<<4)|(s[(x&0x000f)]);
}
inline byte Pt(byte x, byte p[]) {
    byte w = 0;
    for (int i = 0; i < 16; i++)
        w |= ((x>>(i))&1)<<(p[i]);
    return w;
}
inline byte calc0(byte x) {
    return Sboxtransfer[0][Pboxtransfer[0][Sboxtransfer[0][Pboxtransfer[0][Sboxtransfer[0][Pboxtransfer[0][Sboxtransfer[0][x^L[1]]]^L[2]]]^L[3]]]^L[4]]^L[5];
}
inline byte calc1(byte x) {
    return Sboxtransfer[1][Pboxtransfer[1][Sboxtransfer[1][Pboxtransfer[1][Sboxtransfer[1][Pboxtransfer[1][Sboxtransfer[1][x^K[1]]]^K[2]]]^K[3]]]^K[4]]^K[5];
}

byte plaintext[N], ciphertext[N];
int cnt[1<<4][1<<4];
int popcount[1<<16];
byte tmpk[1<<8], tmpkk[1<<8];

void linear_attack(const byte xmask, const byte ymask, const byte bin1, const byte bin2, byte iniK) {
    memset(cnt, 0, sizeof(cnt));
    for (int i = 0; i < N; i++) {
        for (byte L1 = 0; L1 < 1<<4; L1++)
            for (byte L2 = 0; L2 < 1<<4; L2++) {
                byte x = plaintext[i], y = ciphertext[i];
                byte K = (L1<<bin1)|(L2<<bin2)|iniK;
                y ^= K;
                y = Sboxtransfer[0][y];
                byte t = (x&xmask)^(y&ymask);
                cnt[L1][L2] += !(popcount[t]&1);
            }
    }
    for (int L1 = 0; L1 < 1<<4; L1++)
        for (int L2 = 0; L2 < 1<<4; L2++)
            cnt[L1][L2] = abs(cnt[L1][L2]-N/2), tmpk[(L1<<4)|L2] = (L1<<4)|L2;
    sort(tmpk, tmpk+(1<<8), [&](int a, int b) {
                                return cnt[a>>4][a&0xf] > cnt[b>>4][b&0xf];
                            });
    for (int i = 0; i < 1<<8; i++)
        tmpk[i] = ((tmpk[i]>>4)<<bin1)|((tmpk[i]&0xf)<<bin2);
}
inline byte read() {
    byte x = 0; char c = getchar();
    while (c != ' ' && c != '\n') x = (x<<4)|((c >= 'a') ? (c-'a'+10) : (c-'0')), c = getchar();
    return x;
}

inline void work() {
    for (int i = 0; i < N; i++) plaintext[i] = read(), ciphertext[i] = read();
    linear_attack(0x0b00, 0x0505, 8, 0, 0);
    memcpy(tmpkk, tmpk, sizeof(tmpk));
    for (int id = 0; id < 1<<8; id++) {
        byte kk = tmpkk[id];
        if (!id || (kk&0xf00) != (tmpkk[id-1]&0xf00)) linear_attack(0x0c00, 0x5550, 12, 4, kk);
        for (int T = 0; T < 2; T++) {
            byte k = kk|tmpk[T];
            for (int i = 0; i < 1<<16; i++) {
                unsigned int K = (i<<16)|k, T;
                T = K;
                for (int i = 5; i >= 1; i--)
                    ::K[i] = K&0xffff, K >>= 4;
                if (calc1(plaintext[0]) == ciphertext[0] && calc1(plaintext[1]) == ciphertext[1]) {
                    printf("%08x\n", T);
                    return;
                }
            }
        }
    }

    puts("ERROR");

}
int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    for (int i = 0; i < (1<<4); i++) invpiS[piS[i]] = i;
    for (int i = 1; i <= 16; i++) invpiP[piP[i]] = 16-i;
    for (int i = 1; i <= 16; i++) piP[i] = 16-piP[i];
    reverse(piP+1, piP+17);
    reverse(invpiP+1, invpiP+17);
    for (int i = 0; i < 16; i++)
        piP[i] = piP[i+1], invpiP[i] = invpiP[i+1];
    
    for (int i = 0; i < (1<<16); i++) Sboxtransfer[0][i] = St(i, invpiS);
    for (int i = 0; i < (1<<16); i++) Sboxtransfer[1][i] = St(i, piS);
    for (int i = 0; i < (1<<16); i++) Pboxtransfer[0][i] = Pt(i, invpiP);
    for (int i = 0; i < (1<<16); i++) Pboxtransfer[1][i] = Pt(i, piP);
    for (int i = 1; i < (1<<16); i++) popcount[i] = popcount[i-(i&(-i))]+1;

    int t; scanf("%d", &t); getchar();
    while (t--) work();
    return 0;
}
