#include <bits/stdc++.h>
#include <fstream>

using namespace std;

struct phancong
{
    string maphancong;
	string lop;
	string mon;
	string gv;
    int sotiet;
	int len;
    int mucuutien;
	bool ttpc = false;
};

struct tlop
{
    string mon;
    string gv;
};

struct tklop
{
    string lop;
    tlop tiet[31];
};

struct tgv
{
    string mon;
    string lop;
};

struct tkgv
{
    string gv;
    tgv tiet[31];
};

phancong pc[1000];
tklop outlop[20];
tkgv outgv[1000];

string DSmon[] = {"SHCN", "Tin", "TD", "Lý", "CN", "Van", "NN", "Hoa", "Sinh", "Dia", "Su", "GDCD", "Toan"};

int gv_tiet[75][31];
int lop_tiet[35][31];
int slphancong;

void themPhanCong(int i, string _maPhanCong, string _lop, string _mon, string _gv, int _sotiet, int _len);
void nhap();
void xuat_test();
void xuat_tung_pc(int i);
void ghiMucUuTien();
bool dieuKienSapXep(phancong a, phancong b);
void sapXep();
int tongSoTiet(string mon, string gv);
bool check(int i, string lop, int k);

void themPhanCong(int i, string _maPhanCong, string _lop, string _mon, string _gv, int _sotiet, int _len){
    pc[i].maphancong = _maPhanCong;
    pc[i].lop = _lop;
	pc[i].mon = _mon;
	pc[i].gv = _gv;
	pc[i].sotiet = _sotiet;
	pc[i].len = _len;
}

void nhap(){
    ifstream fi("PC12.txt");
    slphancong = 0;
    string _maPhanCong;
    string _lop;
    string _mon;
    string _gv;
    int _sotiet;
    int _len;
    for (int i=1; i<=160; i++){
        fi >> _maPhanCong >> _lop >> _mon >> _gv >> _sotiet >> _len;
        slphancong++;
        themPhanCong(slphancong, _maPhanCong, _lop, _mon, _gv, _sotiet, _len);
    }
    fi.close();
}

void xuat_test(){
    for (int i=1; i<=slphancong; i++){
        cout << i << '\t' << pc[i].maphancong << '\t' << pc[i].lop << '\t' << pc[i].mon << '\t' << pc[i].gv << '\t' << pc[i].sotiet << '\t' << pc[i].len << "\t[" << pc[i].mucuutien << "]\t" << tongSoTiet(pc[i].mon, pc[i].gv) << '\n';
    }
}

void xuat_tung_pc(int i){
    cout << pc[i].maphancong << '\t' << pc[i].lop << '\t' << pc[i].mon << '\t' << pc[i].gv << '\t' << pc[i].sotiet << '\t' << pc[i].len << '\n';
}

void ghiMucUuTien(){
    for (int i=1; i<=slphancong; i++){
        if (pc[i].mon == "SHCN"){
            pc[i].mucuutien = 0;
        }
        else if (pc[i].mon == "Van" || pc[i].mon == "TD"){
            pc[i].mucuutien = 1;
        }
        else if (pc[i].mon == "Toan" || pc[i].mon == "NN"){
            pc[i].mucuutien = 2;
        }
        else{
            pc[i].mucuutien = 3;
        }
    }
}

bool dieuKienSapXep(phancong a, phancong b){ // Muc Uu Tien > Tong So Tiet > Cum
    if (a.mucuutien < b.mucuutien){
        return true;
    }
    else if (a.mucuutien > b.mucuutien){
        return false;
    }
    else if (a.mucuutien == b.mucuutien){
        if (tongSoTiet(a.mon, a.gv) > tongSoTiet(b.mon, b.gv)){
            return true;
        }
        else if (tongSoTiet(a.mon, a.gv) < tongSoTiet(b.mon, b.gv)){
            return false;
        }
        else{
            if (a.len > b.len){
                return true;
            }
            else{
                return false;
            }
        }
    }
}

void sapXep(){
    sort(pc + 1, pc + slphancong + 1, dieuKienSapXep);
}

int tongSoTiet(string mon, string gv){ // Co the toi uu dc buoc nay
    int Tong = 0;
    for (int i=1; i<=slphancong; i++){
        if (pc[i].mon == mon && pc[i].gv == gv){
            Tong += pc[i].len;
            //xuat_tung_pc(i);
        }
    }
    return Tong;
}

void outputtheolop(string lop){
    for (int i=1; i<=slphancong; i++){
        if (pc[i].lop == lop){
            cout << i << '\t' << pc[i].maphancong << '\t' << pc[i].lop << '\t' << pc[i].mon << '\t' << pc[i].gv << '\t' << pc[i].sotiet << '\t' << pc[i].len << "\t[" << pc[i].mucuutien << "]\t" << tongSoTiet(pc[i].mon, pc[i].gv) << '\n';
        }
    }
}

int findsttlop(string lop){
    for (int i=1; i<=10; i++){
        if (outlop[i].lop == lop){
            return i;
            break;
        }
    }
    return 0;
    return 1;
}

bool kiemTraGioLienTiep(string mon, int tietbatdau){ // Van TD
    if (mon != "Van" && mon != "TD"){
        return true;
    }
    else{
        if (tietbatdau%5 == 2){ // Ra choi giua tiet 2 va 3
            return false;
        }
        else{
            return true;
        }
    }
}

bool kiemTraToiDaMon(int tietbatdau, string lop){
    int tietdauthu = ((tietbatdau-1)/5)*5 + 1;
    int sttlop = findsttlop(lop);

    map <string , bool> HashMap;
    int count = 0;
    for (int i=tietdauthu; i<=tietdauthu+4; i++){
        if (lop_tiet[sttlop][i] != 0){ // Tiet da co mon
            if (HashMap[outlop[sttlop].tiet[i].mon] == false){
                count++;
                HashMap[outlop[sttlop].tiet[i].mon] = true;
            }
        }
    }
    if (count >= 4){
        return false;
    }
    else{
        return true;
    }
}

bool kiemTraTrungMon(string mon, int tietbatdau, string lop){
    int tietdauthu = ((tietbatdau-1)/5)*5 + 1;
    int sttlop = findsttlop(lop);

    for (int i=tietdauthu; i<=tietdauthu+4; i++){
        if (lop_tiet[sttlop][i] != 0){ // Tiet da co mon
            if (outlop[sttlop].tiet[i].mon == mon){ // Trung mon
                return false;
            }
        }
    }
    return true;
}

bool kiemTraGioKhongXep(string mon, int tietbatdau){
    if (tietbatdau >= 6 && tietbatdau <= 10 && (mon == "Toan" || mon == "Su" || mon == "Dia" || mon == "Tin" || mon == "GDCD")){
        return false;
    }
    if (tietbatdau >= 11 && tietbatdau <= 15 && (mon == "Van" || mon == "Hoa" || mon == "Lý")){
        return false;
    }
    if (tietbatdau >= 16 && tietbatdau <= 20 && mon == "CN"){
        return false;
    }
    if (tietbatdau >= 21 && tietbatdau <= 25 && (mon == "NN" || mon == "Sinh")){
        return false;
    }
    if (tietbatdau >= 26 && tietbatdau <= 30 && (mon == "TD" || mon == "GDQP")){
        return false;
    }
    return true;
}

int findslot(int i, string lop, int sltiet){ // tra vi tri: 1, 0;
    int sttlop = findsttlop(lop);
    int vitri = 1;
    for (; vitri <= 30; vitri++){
        if (lop_tiet[sttlop][vitri] == 0){ // Tiet trong
            if (check(i, lop, vitri)){
                int j = vitri;
                while (lop_tiet[sttlop][j] == 0){
                    if (j - vitri + 1 == sltiet && j <= (((vitri-1)/5)+1)*5){
                        cerr << vitri << '-' << j << '-' << (((vitri-1)/5)+1)*5 << ' ';
                        return vitri;
                    }
                    j++;
                }
            }
        }
    }
    return 0;
}

void XepPhanCong(int i, string lop, int tietbatdau){ // phan cong i xep vao lop vao tietbatdau
    int sttlop = findsttlop(lop);
    for (int j=0; j < pc[i].len; j++){
        lop_tiet[sttlop][tietbatdau+j] = 1;
        outlop[sttlop].tiet[tietbatdau+j].mon = pc[i].mon;
        outlop[sttlop].tiet[tietbatdau+j].gv = pc[i].gv;
    }
}

bool check(int i, string lop, int k){
    if (kiemTraGioLienTiep(pc[i].mon, k) && kiemTraToiDaMon(k, lop) && kiemTraTrungMon(pc[i].mon, k, lop) && kiemTraGioKhongXep(pc[i].mon, k)){
        return true;
    }
    else{
        return false;
    }
}

void Xepmotlop(string lop){
    for (int i=1; i<=slphancong; i++){
        if (pc[i].lop == lop){
            int sotiet = pc[i].len;
            int k = findslot(i, lop, sotiet);
            //cout << '[' << k << ']' << ' ';
            if (k != 0){
                XepPhanCong(i, lop, k);
            }
        }
    }
}

void outlop_tunglop(string lop){
    int sttlop = findsttlop(lop);
    for (int i=1; i<=30; i++){
        cout << ((i-1)/5) + 2 << '\t' << ((i-1)%5)+1 << '\t' << outlop[sttlop].tiet[i].mon << " - " << outlop[sttlop].tiet[i].gv << '\n';
    }
}

void outlop_hetlop(){
    for (int j=1; j<=30; j++){
        cout << ((j-1)/5) + 2 << '\t' << ((j-1)%5)+1 << '\t';
        for (int i=1; i<=10; i++){
            cout << outlop[i].tiet[j].mon << " - " << outlop[i].tiet[j].gv << '\t';
        }
        cout << '\n';
    }
}

void suaCumTiet(){
    for (int i=1; i<=slphancong; i++){
        if (pc[i].sotiet == 3){
            pc[i].len = pc[i].len;
        }
        else{
            pc[i].len = pc[i].sotiet;
        }
    }
}

int main(){
    freopen("output.txt", "w", stdout);
    memset(lop_tiet, 0, sizeof(lop_tiet));
    nhap();
    suaCumTiet();
    ghiMucUuTien();
    sapXep();
    outlop[1].lop = "12A";
    outlop[2].lop = "12B";
    outlop[3].lop = "12C";
    outlop[4].lop = "12D";
    outlop[5].lop = "12E";
    outlop[6].lop = "12G";
    outlop[7].lop = "12H";
    outlop[8].lop = "12K";
    outlop[9].lop = "12M";
    outlop[10].lop = "12N";
    //xuat_test();
    //outputtheolop("12A");
    Xepmotlop("12A");
    Xepmotlop("12B");
    Xepmotlop("12C");
    Xepmotlop("12D");
    Xepmotlop("12E");
    Xepmotlop("12G");
    Xepmotlop("12H");
    Xepmotlop("12K");
    Xepmotlop("12M");
    Xepmotlop("12N");
    outlop_hetlop();
    //cout << kiemTraTrungMon("NN", 15, "12G") << ' ' << outlop[findsttlop("12G")].tiet[11].mon;
    //cout << lop_tiet[findsttlop("12E")][9];
}
