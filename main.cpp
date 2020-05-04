/*
    *** File input: PC_HK1.txt & PC_HK2.txt
    cấu trúc file: <Mã PC> <Lớp> <Môn> <Mã GV> <Số Tiết> <Cụm>
    vd: PC001 10A Anh GV1024 3 2

    *** File output:
    - TKB của lớp: TKB_lop.txt
    - TKB của gv: TKB_gv.txt
    - Vi phạm của lớp: VP_lop.txt
    - Vi phạm của gv: VP_gv.txt
    - Vi phạm chi tiết: VP_chitiet.txt
    - Vi phạm tổng: VP_tong.txt
    - Số liệu về điểm vi pham (Cải Thiện 2): stat.txt

    *** thay đổi biến HK để chọn học kì
*/

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

int HK = 1;          // 1 : Học kì 1 | 2 : Học kì 2

phancong pc[1000];   // Input phân công
tklop outlop[35];    // Output tkb của lớp
tkgv outgv[1000];    // Output tkb của giáo viên

string DSmonXh[] = {"Van", "Su", "Dia", "GDCD", "Anh"};
string DSlop[] = {"12A","12B","12C","12D","12E","12G","12H","12K","12M","12N","10A","10B","10C","10D","10E","10G","10H","10K","10M","10N","11A","11B","11C","11D","11E","11G","11H","11K","11M","11N","11P","11Q","11T"};

int VP_lop[35][8];
int VP_gv[75][8];

int gv_tiet[75][31];            // Tiết giáo viên trống (0: trống, 1: ko trống)
int lop_tiet[35][31];           // Tiết lớp trống (0: trống, 1: ko trống);
int slphancong, sllop, slgv;    // Số lượng của phân công, lớp, giáo viên;
int slphongtin = 3;             // Số lượng phòng tin học của trường
string file_in = "";
int vp[] = {0, 15, 10, 20, 25, 10, 20, 15}; // Hệ số Vi Phạm
double time_now = 0;

// ===================== Danh Sách Các Hàm ========================
// Thêm 1 phân công
void themPhanCong(int i, string _maPhanCong, string _lop, string _mon, string _gv, int _sotiet, int _len);
// Nhập dữ liệu vào
void nhap();
// Xuất danh sách phân công
void xuat_test();
// Xuất 1 phân công
void xuat_tung_pc(int i);
// Sửa cụm tiết
void suaCumTiet();
// Ghi mức ưu tiên
void ghiMucUuTien();
// Tạo danh sách lớp trong outlop
void taoDanhSachLop();
// Tạo danh sách giáo viên trong outgv
void taoDanhSachGiaoVien();
// Điều kiện để sắp xếp ( Mức ưu tiên => Tổng số tiết => Cụm)
bool dieuKienSapXep(phancong a, phancong b);
// Sắp xếp phân công
void sapXep();
// Tính tổng số tiết
int tongSoTiet(string mon, string gv);
// Xuất danh sách phân công theo lớp
void outputtheolop(string lop);
// Tìm số thứ tự của lớp
int findsttlop(string lop);
// Tìm số thứ tự của giáo viên
int findsttgv(string gv);
// Đếm dòng của file
int countLine(string file);
// Kiểm tra giờ liên tiếp (H7)
bool kiemTraGioLienTiep(int i, int tietbatdau); // Van TD
// Kiểm tra số môn tối đa của buổi (H13)
bool kiemTraToiDaMon(int tietbatdau, string lop);
// Kiểm tra số môn tối đa của buổi (H13) không xét yếu tố môn XH
bool kiemTraToiDaMon2(int tietbatdau, string lop);
// Kiểm tra trùng môn (H8 + S2)
bool kiemTraTrungMon(string mon, int tietbatdau, string lop);
// Kiểm tra trùng môn (H8 + S2)
bool kiemTraTrungMon_2(string mon, int tietbatdau, string lop);
// Kiểm tra giờ không xếp (H2)
bool kiemTraGioKhongXep(string mon, int tietbatdau);
// Kiểm tra trùng giờ giáo viên (H1)
bool kiemTraTrungGioGiaoVien(int k, int tietbatdau);
// Kiểm tra đụng độ phòng học - Tiết tin học (H3)
bool kiemTraDungDoPhongHoc(int i, int tietbatdau);
// Tìm vị trí xếp phân công
int findslot(int i, string lop, int sltiet); // tra vi tri: 1, 0;
// Xếp Phân Công ( Chính )
void XepPhanCong(int i, string lop, int tietbatdau); // phan cong i xep vao lop vao tietbatdau
// tập hợp các điều kiện
bool check(int i, string lop, int k);
// Xếp phân công cho 1 lớp
void Xepmotlop(string lop);
// Xuất output 1 lớp
void outlop_tunglop(string lop);
// Xuất output tất cả lớp
void outlop_hetlop();
// Xuất output 1 giáo viên
void outgv_tunggv(string gv);
// Xuất output tất cả giáo viên
void outgv_hetgv();
// Kiểm tra tất cả phân công có đc xếp ko (H5)
bool checkTatCaPhanCong();
// Kiểm tra tiết lủng
bool checkTietLung();
// Kiểm tra tiết k của lớp i có là tiết lủng ko
bool TietkLung(int k, int i);
// Kiểm tra thay thế tietthaythe(có tiết) => tietbatdau(tiết trống)
bool ChecktietThayThe(int lop, int tietbatdau, int tietthaythe);
// Không kiểm tra trùng môn
bool ChecktietThayThe2(int lop, int tietbatdau, int tietthaythe);
// Thực hiện thay thế tietthaythe(tiết trống) => tietbatdau(có tiêt)
void ThayTheTiet(int i, int tietbatdau, int tietthaythe);
// Thực hiện thay thế cho Cải Tiến 2
void ThayTheTiet_CaiTien2(int i, int tietbatdau, int tietthaythe);
// Tìm tietthaythe thỏa mãn đk (từ tietbatdau lên)
void findtietThayThe(int i, int tietbatdau); // Cum == 1
// Tìm tietthaythe thỏa mãn đk (T7=>T2)
void findtietThayThe_2(int i, int tietbatdau); // Cum == 1
// Kiểm tra tiet_on_2 có phải tiết đơn ko
bool check1tiet(int lop, int tiet_on_2);
// Kiểm tra 1 tiết cho Cải Tiến 2
bool check1tiet_2(int lop, int tiet_on_2);
// Kiểm tra 3-Swap
bool Check3_Swap(int lop, int tiet_on_1, int tiet_off, int tiet_on_2);
bool Check3_Swap_2(int lop, int tiet_on_1, int tiet_off, int tiet_on_2);
// Thêm PC chưa xếp
void ThemPCSau();
// Cải Thiện 1
void CaiThien1();
// Kiểm tra tiết k của lớp i có lủng ko
bool TietkLung2(int k, int i);
// Đếm tiết lủng
int countTietLung();
// Kiểm tra mon có trong thứ sau ko
bool monTrongBuoi(string mon, int lop, int thu);
// Đếm vi phạm S2
int countHocCachMon();
// Đếm chi tiết S2
void VP_S2();
// Đếm vi phạm S3
int countBuoiDaygv();
// Đếm chi tiết S3
void VP_S3();
// Kiểm tra tiết k của gv i có lủng
bool TietkLungGv(int k, int i);
// Đếm vi phạm S4
int countTietLungGv();
// Đếm chi tiết S4
void VP_S4();
// Đếm vi phạm S5
int countBuoiDay1Tiet();
// Đếm chi tiết S5
void VP_S5();
// Đếm vi pham S6
int countTietToiThieu();
// Đếm chi tiết S6
void VP_S6();
// Đếm vi phạm S7
int countToiDaMon();
// Đếm chi tiết S7
void VP_S7();
// Kiêm tra tiết liên tiếp
bool kiemtratietlientiep();
// Kiểm tra giờ liên tiếp
bool kiemtratietlientietlop(int lop);
// Kiểm tra tiết TD 4-5
bool kiemTraTietTheDuc();
// Kiểm tra đụng độ phòng Tin
bool kiemTraDungDoPhongHoc2();
// Tính Điểm Vi Phạm (Tổng)
int TinhDiemViPham();
// Tính Điểm Vi Phạm (Tồng) Xuất file
void TinhDiemViPham_out();
// Tạo bảng vi phạm chi tiết (S)
void create_VP();
// Tính Tổng số lớp của gv dạy của 1 lớp
int tongSoLopGVDayCuaMotLop(string lop);
// So Sách tổng số lớp
bool ComparetongSoLop(string lopa, string lopb);
// Sắp Xếp Danh Sách Lớp
void SortDanhSachLop();
// Xếp Lớp
void Xeplop();
// Số tiết liên tiếp từ tiết tiet
int soTiet(int lop, int tiet);
// Đẩy Tiết Cuối lên (Làm gọn TKB)
void DayTiet();
// Kiểm tra môn có tiết cặp
bool kiemTraMon(string mon, int tiet, int lop);
// Nhập TKB gv (test)
void Nhap_output();
void Nhap_output_2();
// Cải Thiện 2
void CaiThien2();
// ===================================================================

// Thêm 1 phân công
void themPhanCong(int i, string _maPhanCong, string _lop, string _mon, string _gv, int _sotiet, int _len){
    pc[i].maphancong = _maPhanCong;
    pc[i].lop = _lop;
	pc[i].mon = _mon;
	pc[i].gv = _gv;
	pc[i].sotiet = _sotiet;
	pc[i].len = _len;
}

// Nhập dữ liệu vào
void nhap(){
    if (HK == 1){
        file_in = "PC_HK1.txt";
        //file_in = "PC_HK1_QP_tc.txt"; // QP là tiết cặp
        slphancong = countLine(file_in);
    }
    else if (HK == 2){
        file_in = "PC_HK2.txt";
        slphancong = countLine(file_in);
    }
    ifstream fi(file_in);
    string _maPhanCong;
    string _lop;
    string _mon;
    string _gv;
    int _sotiet;
    int _len;
    for (int i=1; i<=slphancong; i++){
        fi >> _maPhanCong >> _lop >> _mon >> _gv >> _sotiet >> _len;
        themPhanCong(i, _maPhanCong, _lop, _mon, _gv, _sotiet, _len);
    }
    fi.close();
}

// Xuất danh sách phân công
void xuat_test(){
    for (int i=1; i<=slphancong; i++){
        cout << i << '\t' << pc[i].maphancong << '\t' << pc[i].lop << '\t' << pc[i].mon << '\t' << pc[i].gv << '\t' << pc[i].sotiet << '\t' << pc[i].len << "\t[" << pc[i].mucuutien << "]\t" << tongSoTiet(pc[i].mon, pc[i].gv) << '\n';
    }
}

// Xuất 1 phân công
void xuat_tung_pc(int i){
    cout << pc[i].maphancong << '\t' << pc[i].lop << '\t' << pc[i].mon << '\t' << pc[i].gv << '\t' << pc[i].sotiet << '\t' << pc[i].len << '\n';
}

// Sửa cụm tiết
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

// Ghi mức ưu tiên
void ghiMucUuTien(){
    for (int i=1; i<=slphancong; i++){
        if (pc[i].mon == "SHL"){
            pc[i].mucuutien = 0;
        }
        else if (pc[i].mon == "Van" || pc[i].mon == "TD" ){
            pc[i].mucuutien = 1;
        }
        else if (pc[i].mon == "Toan" || pc[i].mon == "Anh" || pc[i].mon == "AVNN"){
            pc[i].mucuutien = 2;
        }
        else{
            pc[i].mucuutien = 3;
        }
    }
}

// Tạo danh sách lớp trong outlop
void taoDanhSachLop(){
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

    outlop[11].lop = "10A";
    outlop[12].lop = "10B";
    outlop[13].lop = "10C";
    outlop[14].lop = "10D";
    outlop[15].lop = "10E";
    outlop[16].lop = "10G";
    outlop[17].lop = "10H";
    outlop[18].lop = "10K";
    outlop[19].lop = "10M";
    outlop[20].lop = "10N";

    outlop[21].lop = "11A";
    outlop[22].lop = "11B";
    outlop[23].lop = "11C";
    outlop[24].lop = "11D";
    outlop[25].lop = "11E";
    outlop[26].lop = "11G";
    outlop[27].lop = "11H";
    outlop[28].lop = "11K";
    outlop[29].lop = "11M";
    outlop[30].lop = "11N";
    outlop[31].lop = "11P";
    outlop[32].lop = "11Q";
    outlop[33].lop = "11T";
    sllop = 33;
}

// Tạo danh sách giáo viên trong outgv
void taoDanhSachGiaoVien(){
    set <string> myset;
    for (int i=1; i<=slphancong; i++){
        myset.insert(pc[i].gv);
    }
    int stt = 1;
    for (set <string>::iterator it=myset.begin(); it!=myset.end(); ++it){
        outgv[stt].gv = *it;
        stt++;
    }
    slgv = myset.size();
}

// Điều kiện để sắp xếp ( Mức ưu tiên => Tổng số tiết => Cụm)
bool dieuKienSapXep(phancong a, phancong b){
    if (a.mucuutien < b.mucuutien){
        return true;
    }
    else if (a.mucuutien > b.mucuutien){
        return false;
    }
    else{
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

// Sắp xếp phân công
void sapXep(){
    sort(pc + 1, pc + slphancong + 1, dieuKienSapXep);
}

// Tính tổng số tiết
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

// Xuất danh sách phân công theo lớp
void outputtheolop(string lop){
    for (int i=1; i<=slphancong; i++){
        if (pc[i].lop == lop){
            cout << i << '\t' << pc[i].maphancong << '\t' << pc[i].lop << '\t' << pc[i].mon << '\t' << pc[i].gv << '\t' << pc[i].sotiet << '\t' << pc[i].len << "\t[" << pc[i].mucuutien << "]\t" << tongSoTiet(pc[i].mon, pc[i].gv) << '\n';
        }
    }
}

// Tìm số thứ tự của lớp
int findsttlop(string lop){
    for (int i=1; i<=sllop; i++){
        if (outlop[i].lop == lop){
            return i;
            break;
        }
    }
    return 0;
}

// Tìm số thứ tự của giáo viên
int findsttgv(string gv){
    for (int i=1; i<=slgv; i++){
        if (outgv[i].gv == gv){
            return i;
        }
    }
    return 0;
}

// Đếm dòng của file
int countLine(string file){
    string _;
    ifstream _f(file);
    int line = 0;
    while (!_f.eof()){
        getline (_f, _);
        if (_ != ""){
            line++;
        }
    }
    return line;
}

// Kiểm tra giờ liên tiếp (H7)
bool kiemTraGioLienTiep(int i, int tietbatdau){ // Van TD
    if (pc[i].mon != "Van" && pc[i].mon != "TD"){
        return true;
    }
    else{
        if (tietbatdau%5 == 2 && pc[i].len > 1){ // Ra choi giua tiet 2 va 3
            return false;
        }
        else{
            return true;
        }
    }
}

// Kiểm tra số môn tối đa của buổi (H13)
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
        int t = 0;
        for (map<string, bool>::iterator it=HashMap.begin(); it!=HashMap.end(); ++it){
            for (int i=0; i<=4; i++){
                if (it->first == DSmonXh[i]){
                    t++;
                    break;
                }
            }
        }
        if (t == 5){
            return false;
        }
        else{
            return true;
        }

    }
    else{
        return true;
    }
}

// Kiểm tra số môn tối đa của buổi (H13) không xét yếu tố môn XH
bool kiemTraToiDaMon2(int tietbatdau, string lop){
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
    if (count > 4){
        return false;
    }
    else{
        return true;
    }
}

// Kiểm tra trùng môn (H8 + S2)
bool kiemTraTrungMon(string mon, int tietbatdau, string lop){
    int tietdauthu = ((tietbatdau-1)/5)*5 + 1;
    int sttlop = findsttlop(lop);

    for (int i=tietdauthu; i<=tietdauthu+4; i++){
        if (lop_tiet[sttlop][i] != 0){ // Tiet da co mon
            if (outlop[sttlop].tiet[i].mon == mon){ // Trung mon
                if (i - tietbatdau == 1){
                    if (i == tietdauthu + 4 || outlop[sttlop].tiet[i+1].mon != outlop[sttlop].tiet[i].mon){
                        return true;
                    }
                }
                else if (tietbatdau - i == 1){
                    if (i == tietdauthu || outlop[sttlop].tiet[i-1].mon != outlop[sttlop].tiet[i].mon){
                        return true;
                    }
                }
                else{
                    return false;
                }
            }
        }
    }
    return true;
}

// Kiểm tra trùng môn (H8 + S2)
bool kiemTraTrungMon_2(string mon, int tietbatdau, string lop){
    int tietdauthu = ((tietbatdau-1)/5)*5 + 1;
    int sttlop = findsttlop(lop);

    for (int i=tietdauthu; i<=tietdauthu+4; i++){
        if (lop_tiet[sttlop][i] != 0){ // Tiet da co mon
            if (tietbatdau != i && outlop[sttlop].tiet[i].mon == mon){ // Trung mon
                return false;
            }
        }
    }
    return true;
}

// Kiểm tra giờ không xếp (H2)
bool kiemTraGioKhongXep(string mon, int tietbatdau){
    if (tietbatdau >= 6 && tietbatdau <= 10 && (mon == "Toan" || mon == "Su" || mon == "Dia" /*|| mon == "Tin" */|| mon == "GDCD")){
        return false;
    }
    if (tietbatdau >= 11 && tietbatdau <= 15 && (mon == "Van" || mon == "Hoa" || mon == "Lý")){
        return false;
    }
    if (tietbatdau >= 16 && tietbatdau <= 20 && mon == "CN"){
        return false;
    }
    if (tietbatdau >= 21 && tietbatdau <= 25 && (mon == "Anh" || mon == "Sinh")){
        return false;
    }
    if (tietbatdau >= 26 && tietbatdau <= 30 && (/*mon == "TD" || */mon == "GDQP")){
        return false;
    }
    return true;
}

// Kiểm tra trùng giờ giáo viên (H1)
bool kiemTraTrungGioGiaoVien(int k, int tietbatdau){
    for (int i=1; i<=sllop; i++){
        for (int j=0; j<pc[k].len; j++){
            if (lop_tiet[i][tietbatdau + j] == 1 && outlop[i].tiet[tietbatdau + j].gv == pc[k].gv){ // Co tiet va trung gv
                return false;
            }
        }
    }
    return true;
}

// Kiểm tra đụng độ phòng học - Tiết tin học (H3)
bool kiemTraDungDoPhongHoc(int i, int tietbatdau){
    if (pc[i].mon == "Tin"){
        for (int k=0; k<pc[i].len; k++){
            int count = 1;
            for (int j=1; j<=sllop; j++){
                if (findsttlop(pc[i].lop) != j && lop_tiet[j][tietbatdau + k] == 1){
                    if (outlop[j].tiet[tietbatdau+k].mon == "Tin"){
                        count++;
                        if (count > slphongtin){
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }
    return true;
}

// Tìm vị trí xếp phân công
int findslot(int i, string lop, int sltiet){ // tra vi tri: 1, 0;
    int sttlop = findsttlop(lop);
    int vitri = 1;
    for (; vitri <= 30; vitri++){
        if (lop_tiet[sttlop][vitri] == 0){ // Tiet trong
            if (check(i, lop, vitri)){
                int j = vitri;
                while (lop_tiet[sttlop][j] == 0){
                    if (j - vitri + 1 == sltiet && j <= (((vitri-1)/5)+1)*5){
                        //cerr << vitri << '-' << j << '-' << (((vitri-1)/5)+1)*5 << ' ';
                        return vitri;
                    }
                    j++;
                }
            }
        }
    }
    return 0;
}

// Xếp Phân Công ( Chính )
void XepPhanCong(int i, string lop, int tietbatdau){ // phan cong i xep vao lop vao tietbatdau
    int sttlop = findsttlop(lop);
    int sttgv = findsttgv(pc[i].gv);
    for (int j=0; j < pc[i].len; j++){
        lop_tiet[sttlop][tietbatdau+j] = 1;
        outlop[sttlop].tiet[tietbatdau+j].mon = pc[i].mon;
        outlop[sttlop].tiet[tietbatdau+j].gv = pc[i].gv;

        pc[i].ttpc = true;
        outgv[sttgv].tiet[tietbatdau+j].mon = pc[i].mon;
        outgv[sttgv].tiet[tietbatdau+j].lop = pc[i].lop;
    }
}

// tập hợp các điều kiện
bool check(int i, string lop, int k){
    if (tongSoTiet(pc[i].mon, pc[i].gv) < 25){
        if (kiemTraGioLienTiep(i, k) && kiemTraTrungMon(pc[i].mon, k, lop) && kiemTraGioKhongXep(pc[i].mon, k) && kiemTraTrungGioGiaoVien(i, k) && kiemTraDungDoPhongHoc(i, k)){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        if (kiemTraGioLienTiep(i, k) && kiemTraTrungMon(pc[i].mon, k, lop) && kiemTraTrungGioGiaoVien(i, k) && kiemTraDungDoPhongHoc(i, k)){
            return true;
        }
        else{
            return false;
        }
    }
}

// Xếp phân công cho 1 lớp
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

// Xuất output 1 lớp
void outlop_tunglop(string lop){
    int sttlop = findsttlop(lop);
    for (int i=1; i<=30; i++){
        cout << ((i-1)/5) + 2 << '\t' << ((i-1)%5)+1 << '\t' << outlop[sttlop].tiet[i].mon << " - " << outlop[sttlop].tiet[i].gv << '\n';
    }
}

// Xuất output tất cả lớp
void outlop_hetlop(){
    ofstream fo("TKB_lop.txt");

    fo << "lop" << "\ttiet";
    for (int i=1; i<=sllop; i++){
        fo << '\t' << outlop[i].lop;
    }
    fo << '\n';

    for (int j=1; j<=30; j++){
        fo << ((j-1)/5) + 2 << '\t' << ((j-1)%5)+1 << '\t';
        for (int i=1; i<=sllop; i++){
            fo << outlop[i].tiet[j].mon << " - " << outlop[i].tiet[j].gv << '\t';
        }
        fo << '\n';
    }
    fo.close();
}

// Xuất output 1 giáo viên
void outgv_tunggv(string gv){
    int sttgv = findsttgv(gv);
    for (int i=1; i<=30; i++){
        cout << ((i-1)/5) + 2 << '\t' << ((i-1)%5)+1 << '\t' << outgv[sttgv].tiet[i].mon << "-" << outgv[sttgv].tiet[i].lop << '\n';
    }
}

// Xuất output tất cả giáo viên
void outgv_hetgv(){
    ofstream fo("TKB_gv.txt");

    fo << "lop" << "\ttiet";
    for (int i=1; i<=slgv; i++){
        fo << '\t' << outgv[i].gv;
    }
    fo << '\n';

    for (int j=1; j<=30; j++){
        fo << ((j-1)/5) + 2 << '\t' << ((j-1)%5)+1 << '\t';
        for (int i=1; i<=slgv; i++){
            fo << outgv[i].tiet[j].mon << " - " << outgv[i].tiet[j].lop << '\t';
        }
        fo << '\n';
    }
    fo.close();
}

// Kiểm tra tất cả phân công có đc xếp ko (H5)
bool checkTatCaPhanCong(){
    bool ck = true;
    for (int i=1; i<=slphancong; i++){
        if (pc[i].ttpc == false){
            ck = false;
            xuat_tung_pc(i);
        }
    }
    if (!ck){
        return false;
    }
    return true;
}

// Kiểm tra tiết lủng
bool checkTietLung(){
    for (int i=1; i<=sllop; i++){
        for (int j=1; j<=30; j++){
            if (lop_tiet[i][j] == 0){
                if (j%5 != 0 && j%5 != 1){
                    bool c1 = false;
                    bool c2 = false;
                    for (int jt=j; jt >= ((j-1)/5)*5 + 1; jt--){
                        if (lop_tiet[i][jt] == 1){
                            c1 = true;
                        }
                    }
                    for (int js=j; js <= ((j-1)/5 + 1)*5; js++){
                        if (lop_tiet[i][js] == 1){
                            c2 = true;
                        }
                    }
                    if (c1 & c2){
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

// Kiểm tra tiết k của lớp i có là tiết lủng ko
bool TietkLung(int k, int i){
    if (lop_tiet[i][k] == 0){ // Tiết trống
        if (k == ((k-1)/5)*5 + 1){ // Tiet đầu trống
            return true;
        }
        bool c1 = false;
        bool c2 = false;
        int k_truoc = k;
        while (k_truoc >= ((k-1)/5)*5 + 1){
            if (lop_tiet[i][k_truoc] == 1){ // Tiết trước đó có tiết
                c1 = true;
                break;
            }
            k_truoc--;
        }
        int k_sau = k;
        while (k_sau <= ((k-1)/5 + 1)*5){
            if (lop_tiet[i][k_sau] == 1){ // Tiết sau đó có tiết
                c2 = true;
                break;
            }
            k_sau++;
        }
        if (c1 & c2){
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

// Kiểm tra thay thế tietthaythe(có tiết) => tietbatdau(tiết trống)
bool ChecktietThayThe(int lop, int tietbatdau, int tietthaythe){
    // Kiểm tra trùng giáo viên
    for (int i=1; i<=sllop; i++){
        if (i != lop){
            if (lop_tiet[i][tietbatdau] == 1 && outlop[i].tiet[tietbatdau].gv == outlop[lop].tiet[tietthaythe].gv){ // Co tiet va trung gv
                return false;
            }
        }
    }

    // Kiểm tra giờ không xếp
    if (!kiemTraGioKhongXep(outlop[lop].tiet[tietthaythe].mon, tietbatdau)){
        return false;
    }

    // Kiểm tra trùng môn không cặp
    if (!kiemTraTrungMon_2(outlop[lop].tiet[tietthaythe].mon, tietbatdau, outlop[lop].lop)){
        return false;
    }

    // Kiểm tra đụng độ phòng học
    if (outlop[lop].tiet[tietthaythe].mon == "Tin"){
        int count = 1;
        for (int j=1; j<=sllop; j++){
            if (lop != j && lop_tiet[j][tietbatdau] == 1){
                if (outlop[j].tiet[tietbatdau].mon == "Tin"){
                    count++;
                    if (count > slphongtin){
                        return false;
                    }
                }
            }
        }
    }

    // Kiểm tra môn TD tiết 4-5
    if (outlop[lop].tiet[tietthaythe].mon == "TD" && tietbatdau%5 == 0){
        return false;
    }

    return true;
}

// Không kiểm tra trùng môn
bool ChecktietThayThe2(int lop, int tietbatdau, int tietthaythe){
    // Kiểm tra trùng giáo viên
    for (int i=1; i<=sllop; i++){
        if (i != lop){
            if (lop_tiet[i][tietbatdau] == 1 && outlop[i].tiet[tietbatdau].gv == outlop[lop].tiet[tietthaythe].gv){ // Co tiet va trung gv
                return false;
            }
        }
    }

    // Kiểm tra giờ không xếp
    if (!kiemTraGioKhongXep(outlop[lop].tiet[tietthaythe].mon, tietbatdau)){
        return false;
    }

    // Kiểm tra đụng độ phòng học
    if (outlop[lop].tiet[tietthaythe].mon == "Tin"){
        int count = 1;
        for (int j=1; j<=sllop; j++){
            if (lop != j && lop_tiet[j][tietbatdau] == 1){
                if (outlop[j].tiet[tietbatdau].mon == "Tin"){
                    count++;
                    if (count > slphongtin){
                        return false;
                    }
                }
            }
        }
    }

    // Kiểm tra môn TD tiết 4-5
    if (outlop[lop].tiet[tietthaythe].mon == "TD" && tietbatdau%5 == 0){
        return false;
    }

    return true;
}

// Thực hiện thay thế tietthaythe(tiết trống) => tietbatdau(có tiêt)
void ThayTheTiet(int i, int tietbatdau, int tietthaythe){
    lop_tiet[i][tietthaythe] = 0;
    lop_tiet[i][tietbatdau] = 1;
    swap(outgv[findsttgv(outlop[i].tiet[tietthaythe].gv)].tiet[tietbatdau], outgv[findsttgv(outlop[i].tiet[tietthaythe].gv)].tiet[tietthaythe]);
    swap(outlop[i].tiet[tietbatdau], outlop[i].tiet[tietthaythe]);
}

// Thực hiện thay thế cho Cải Tiến 2
void ThayTheTiet_CaiTien2(int i, int tietbatdau, int tietthaythe){
    if (findsttgv(outlop[i].tiet[tietthaythe].gv) != findsttgv(outlop[i].tiet[tietbatdau].gv)){
        swap(outgv[findsttgv(outlop[i].tiet[tietthaythe].gv)].tiet[tietbatdau], outgv[findsttgv(outlop[i].tiet[tietthaythe].gv)].tiet[tietthaythe]);
        swap(outgv[findsttgv(outlop[i].tiet[tietbatdau].gv)].tiet[tietbatdau], outgv[findsttgv(outlop[i].tiet[tietbatdau].gv)].tiet[tietthaythe]);
        swap(outlop[i].tiet[tietbatdau], outlop[i].tiet[tietthaythe]);
    }
    else{
        swap(outgv[findsttgv(outlop[i].tiet[tietthaythe].gv)].tiet[tietbatdau], outgv[findsttgv(outlop[i].tiet[tietthaythe].gv)].tiet[tietthaythe]);
        swap(outlop[i].tiet[tietbatdau], outlop[i].tiet[tietthaythe]);
    }
}

// Tìm tietthaythe thỏa mãn đk (từ tietbatdau lên)
void findtietThayThe(int i, int tietbatdau){ // Cum == 1
    for (int tiet = ((tietbatdau-1)/5)*5; tiet >= 5; tiet -= 5){
        if (lop_tiet[i][tiet] == 1 && outlop[i].tiet[tiet].mon != outlop[i].tiet[tiet-1].mon){ // Có tiêt và cum = 1
            if (ChecktietThayThe(i, tietbatdau, tiet)){
                ThayTheTiet(i, tietbatdau, tiet);
                break;
            }
        }
    }
}

// Tìm tietthaythe thỏa mãn đk (T7=>T2)
void findtietThayThe_2(int i, int tietbatdau){ // Cum == 1
    for (int tiet = 30; tiet >= 5; tiet -= 5){
        if (lop_tiet[i][tiet] == 1 && outlop[i].tiet[tiet].mon != outlop[i].tiet[tiet-1].mon){ // Có tiêt và cum = 1
            if (ChecktietThayThe(i, tietbatdau, tiet)){
                ThayTheTiet(i, tietbatdau, tiet);
                break;
            }
        }
    }
}

// Kiểm tra tiet_on_2 có phải tiết đơn ko
bool check1tiet(int lop, int tiet_on_2){
    if (tiet_on_2%5 == 1){
        if (outlop[lop].tiet[tiet_on_2].mon != outlop[lop].tiet[tiet_on_2 + 1].mon){
            return true;
        }
        else{
            return false;
        }
    }
    else if (tiet_on_2%5 == 0){
        if (outlop[lop].tiet[tiet_on_2].mon != outlop[lop].tiet[tiet_on_2 - 1].mon){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        if (outlop[lop].tiet[tiet_on_2].mon != outlop[lop].tiet[tiet_on_2 + 1].mon && outlop[lop].tiet[tiet_on_2].mon != outlop[lop].tiet[tiet_on_2 - 1].mon){
            return true;
        }
        else{
            return false;
        }
    }
}

// Kiểm tra 1 tiết cho Cải Tiến 2
bool check1tiet_2(int lop, int tiet_on_2){
    if (outlop[lop].tiet[tiet_on_2].mon == "Van" && check1tiet(lop, tiet_on_2)){
        return true;
    }
    else if (outlop[lop].tiet[tiet_on_2].mon != "TD" && outlop[lop].tiet[tiet_on_2].mon != "Tin"){
        return true;
    }
    return false;
}

// Kiểm tra 3-Swap
bool Check3_Swap(int lop, int tiet_on_1, int tiet_off, int tiet_on_2){
    if (lop_tiet[lop][tiet_on_1] == 1 && lop_tiet[lop][tiet_off] == 0 && lop_tiet[lop][tiet_on_2] == 1 && check1tiet(lop, tiet_on_2)){
        if (ChecktietThayThe(lop, tiet_off, tiet_on_2) && ChecktietThayThe(lop, tiet_on_2, tiet_on_1)){
            return true;
        }
    }
    return false;
}

bool Check3_Swap_2(int lop, int tiet_on_1, int tiet_off, int tiet_on_2){
    if (lop_tiet[lop][tiet_on_1] == 1 && lop_tiet[lop][tiet_off] == 0 && lop_tiet[lop][tiet_on_2] == 1 && check1tiet/*_2*/(lop, tiet_on_2)){
        if (ChecktietThayThe(lop, tiet_off, tiet_on_2) && ChecktietThayThe(lop, tiet_on_2, tiet_on_1)){
            return true;
        }
    }
    return false;
}

// Thêm PC chưa xếp
void ThemPCSau(){
    for (int i=1; i<=slphancong; i++){
        if (pc[i].ttpc == false){
            int sotiet = pc[i].len;
            int k = findslot(i, pc[i].lop, sotiet);
            //cout << '[' << k << ']' << ' ';
            if (k != 0){
                XepPhanCong(i, pc[i].lop, k);
            }
        }
    }
}

// Cải Thiện 1
void CaiThien1(){
    /////////////// Đẩy Tiết và thêm PC //////////////////////////////
    // Từ thứ 6 => thứ 2
    for (int i=1; i<=sllop; i++){
        for (int tiet=25; tiet>=1; tiet--){
            if (TietkLung(tiet, i)){
                //cout << tiet << ' ' << outlop[i].lop << '\n';
                findtietThayThe(i, tiet);
            }
        }
    }

    // Đẩy tiết thứ 7

    for (int i=1; i<=sllop; i++){
        for (int tiet=30; tiet>=26; tiet--){
            if (lop_tiet[i][tiet] == 1){ // Có tiết

                //  Tìm tiết trống thay thế
                for (int j=26; j>=1; j--){
                    if (lop_tiet[i][j] == 0){ // Tiết trống
                        if (ChecktietThayThe(i, j, tiet)){
                            ThayTheTiet(i, j, tiet);
                            break;
                        }
                    }
                }
            }
        }
    }
    // Fill Thứ 6

    for (int i=1; i<=sllop; i++){
        for (int tiet=21; tiet<=25; tiet++){
            if (lop_tiet[i][tiet] == 0){ // Trống tiết
                findtietThayThe(i, tiet);
            }
        }
    }

    // Đẩy tiết thứ 7

    for (int i=1; i<=sllop; i++){
        for (int tiet=30; tiet>=26; tiet--){
            if (lop_tiet[i][tiet] == 1){ // Có tiết

                //  Tìm tiết trống thay thế
                for (int j=26; j>=1; j--){
                    if (lop_tiet[i][j] == 0){ // Tiết trống
                        if (ChecktietThayThe(i, j, tiet)){
                            ThayTheTiet(i, j, tiet);
                            break;
                        }
                    }
                }
            }
        }
    }

    // Dồn thứ 7

    for (int i=1; i<=sllop; i++){
        for (int tiet=30; tiet>=26; tiet--){
            if (lop_tiet[i][tiet] == 1){ // Có tiết
                //  Tìm tiết trống thay thế
                for (int j=26; j<=tiet; j++){
                    if (lop_tiet[i][j] == 0){ // Tiết trống
                        if (ChecktietThayThe2(i, j, tiet)){
                            ThayTheTiet(i, j, tiet);
                            break;
                        }
                    }
                }
            }
        }
    }

    ThemPCSau();

    //////////////////////////////////////////////////////////////////////

    ////////////////////////////// 3-Swap ////////////////////////////////
    // 3-Swap
    for (int lop=1; lop<=sllop; lop++){
        for (int tiet_on_1=30; tiet_on_1>=26; tiet_on_1--){
            if (lop_tiet[lop][tiet_on_1] == 1){ // Có tiết 1
                for (int tiet_off=2; tiet_off<=25; tiet_off++){
                    if (lop_tiet[lop][tiet_off] == 0){ // Tiết trống
                        for (int tiet_on_2=2; tiet_on_2 <= 25; tiet_on_2++){
                            if (lop_tiet[lop][tiet_on_2] == 1){ // Có Tiết 2
                                if (Check3_Swap(lop, tiet_on_1, tiet_off, tiet_on_2)){
                                    ThayTheTiet(lop, tiet_off, tiet_on_2);
                                    ThayTheTiet(lop, tiet_on_2, tiet_on_1);
                                    //cout << lop << ' ' << tiet_off << ' ' << tiet_on_2 << '\n';
                                    //cout << lop << ' ' << tiet_on_2 << ' ' << tiet_on_1 << '\n';
                                    //cout << "||||||||||||||||||||||\n";
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for (int lop=1; lop<=sllop; lop++){
        for (int tiet_on_1=30; tiet_on_1>=26; tiet_on_1--){
            if (lop_tiet[lop][tiet_on_1] == 1){ // Có tiết 1
                for (int tiet_off=2; tiet_off<=25; tiet_off++){
                    if (lop_tiet[lop][tiet_off] == 0){ // Tiết trống
                        for (int tiet_on_2=2; tiet_on_2 <= 25; tiet_on_2++){
                            if (lop_tiet[lop][tiet_on_2] == 1){ // Có Tiết 2
                                if (Check3_Swap_2(lop, tiet_on_1, tiet_off, tiet_on_2)){
                                    ThayTheTiet(lop, tiet_off, tiet_on_2);
                                    ThayTheTiet(lop, tiet_on_2, tiet_on_1);
                                    //cout << lop << ' ' << tiet_off << ' ' << tiet_on_2 << '\n';
                                    //cout << lop << ' ' << tiet_on_2 << ' ' << tiet_on_1 << '\n';
                                    //cout << "||||||||||||||||||||||\n";
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Dồn thứ 7

    for (int i=1; i<=sllop; i++){
        for (int tiet=30; tiet>=26; tiet--){
            if (lop_tiet[i][tiet] == 1){ // Có tiết
                //  Tìm tiết trống thay thế
                for (int j=26; j<=tiet; j++){
                    if (lop_tiet[i][j] == 0){ // Tiết trống
                        if (ChecktietThayThe2(i, j, tiet)){
                            ThayTheTiet(i, j, tiet);
                            break;
                        }
                    }
                }
            }
        }
    }

    bool change = true;

    while (change){

        change = false;
        ThemPCSau();

        for (int lop=1; lop<=sllop; lop++){
            for (int tiet_on_1=30; tiet_on_1>=26; tiet_on_1--){
                if (lop_tiet[lop][tiet_on_1] == 1){ // Có tiết 1
                    for (int tiet_off=2; tiet_off<=tiet_on_1; tiet_off++){
                        if (lop_tiet[lop][tiet_off] == 0){ // Tiết trống
                            for (int tiet_on_2=2; tiet_on_2 <= 25; tiet_on_2++){
                                if (lop_tiet[lop][tiet_on_2] == 1){ // Có Tiết 2
                                    if (Check3_Swap_2(lop, tiet_on_1, tiet_off, tiet_on_2)){
                                        change = true;
                                        ThayTheTiet(lop, tiet_off, tiet_on_2);
                                        ThayTheTiet(lop, tiet_on_2, tiet_on_1);
                                        //cout << lop << ' ' << tiet_off << ' ' << tiet_on_2 << '\n';
                                        //cout << lop << ' ' << tiet_on_2 << ' ' << tiet_on_1 << '\n';
                                        //cout << "||||||||||||||||||||||\n";
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // Dồn thứ 7

        for (int i=1; i<=sllop; i++){
            for (int tiet=30; tiet>=26; tiet--){
                if (lop_tiet[i][tiet] == 1){ // Có tiết
                    //  Tìm tiết trống thay thế
                    for (int j=26; j<=tiet; j++){
                        if (lop_tiet[i][j] == 0){ // Tiết trống
                            if (ChecktietThayThe2(i, j, tiet)){
                                change = true;
                                ThayTheTiet(i, j, tiet);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    bool change_2 = true;

    while (change_2){

        change_2 = false;
        ThemPCSau();

        for (int lop=1; lop<=sllop; lop++){
            for (int tiet_on_1=30; tiet_on_1>=2; tiet_on_1--){
                if (lop_tiet[lop][tiet_on_1] == 1){ // Có tiết 1
                    for (int tiet_off=2; tiet_off<=tiet_on_1; tiet_off++){
                        if (lop_tiet[lop][tiet_off] == 0){ // Tiết trống
                            for (int tiet_on_2=2; tiet_on_2 <= 30; tiet_on_2++){
                                if (lop_tiet[lop][tiet_on_2] == 1){ // Có Tiết 2
                                    if (Check3_Swap_2(lop, tiet_on_1, tiet_off, tiet_on_2) && tiet_on_1 != tiet_on_2 && check1tiet(lop, tiet_on_1)){
                                        change_2 = true;
                                        ThayTheTiet(lop, tiet_off, tiet_on_2);
                                        ThayTheTiet(lop, tiet_on_2, tiet_on_1);
                                        //cout << lop << ' ' << tiet_off << ' ' << tiet_on_2 << '\n';
                                        //cout << lop << ' ' << tiet_on_2 << ' ' << tiet_on_1 << '\n';
                                        //cout << "||||||||||||||||||||||\n";
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // Dồn thứ 7

        for (int i=1; i<=sllop; i++){
            for (int tiet=30; tiet>=26; tiet--){
                if (lop_tiet[i][tiet] == 1){ // Có tiết
                    //  Tìm tiết trống thay thế
                    for (int j=26; j<=tiet; j++){
                        if (lop_tiet[i][j] == 0){ // Tiết trống
                            if (ChecktietThayThe2(i, j, tiet)){
                                change_2 = true;
                                ThayTheTiet(i, j, tiet);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    /////////////////////////////////////////

    //////////// Làm gọn TKB ////////////////

     // Từ thứ 6 => thứ 2
    for (int i=1; i<=sllop; i++){
        for (int tiet=30; tiet>=1; tiet--){
            if (TietkLung2(tiet, i)){
                //cout << tiet << ' ' << outlop[i].lop << '\n';
                findtietThayThe_2(i, tiet);
            }
        }
    }

    // Đẩy tiết thứ 7

    for (int i=1; i<=sllop; i++){
        for (int tiet=30; tiet>=26; tiet--){
            if (lop_tiet[i][tiet] == 1){ // Có tiết
                bool ck = true;
                //  Tìm tiết trống thay thế
                for (int j=26; j>=1; j--){
                    if (lop_tiet[i][j] == 0){ // Tiết trống
                        if (ChecktietThayThe(i, j, tiet)){
                            ThayTheTiet(i, j, tiet);
                            ck = false;
                            break;
                        }
                    }
                }
                if (ck){
                    break;
                }
            }
        }
    }
    ///////////////////////////
}

// Kiểm tra tiết k của lớp i có lủng ko
bool TietkLung2(int k, int i){
    if (lop_tiet[i][k] == 0){ // Tiết trống
        bool c1 = false;
        bool c2 = false;
        int k_truoc = k;
        while (k_truoc >= ((k-1)/5)*5 + 1){
            if (lop_tiet[i][k_truoc] == 1){ // Tiết trước đó có tiết
                c1 = true;
                break;
            }
            k_truoc--;
        }
        int k_sau = k;
        while (k_sau <= ((k-1)/5 + 1)*5){
            if (lop_tiet[i][k_sau] == 1){ // Tiết sau đó có tiết
                c2 = true;
                break;
            }
            k_sau++;
        }
        if (c1 & c2){
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

// Đếm tiết lủng
int countTietLung(){
    int ct = 0;
    for (int i=1; i<=sllop; i++){
        for (int tiet=30; tiet>=1; tiet--){
            if (TietkLung2(tiet, i)){
                ct++;
            }
        }
    }
    return ct;
}

// Kiểm tra mon có trong thứ sau ko
bool monTrongBuoi(string mon, int lop, int thu){
    for (int tiet = (thu-1)*5 + 1; tiet <= thu*5; tiet++){
        if (outlop[lop].tiet[tiet].mon == mon){
            return true;
        }
    }
    return false;
}

// Đếm vi phạm S2
int countHocCachMon(){
    int ct = 0;
    for (int lop=1; lop<=sllop; lop++){
        for (int i=1; i<=25; i++){
            if (outlop[lop].tiet[i].mon != ""){
                if (i % 5 == 0 || (i % 5 != 0 && outlop[lop].tiet[i].mon != outlop[lop].tiet[i + 1].mon)){
                    if (monTrongBuoi(outlop[lop].tiet[i].mon, lop, (i-1)/5 + 2)){
                        ct++;
                    }
                }
            }
        }
    }
    return ct;
}

// Đếm chi tiết S2
void VP_S2(){
    ofstream fp;
    fp.open("VP_chitiet.txt", ios::out | ios::app);
    for (int lop=1; lop<=sllop; lop++){
        for (int i=1; i<=25; i++){
            if (outlop[lop].tiet[i].mon != ""){
                if (i % 5 == 0 || (i % 5 != 0 && outlop[lop].tiet[i].mon != outlop[lop].tiet[i + 1].mon)){
                    if (monTrongBuoi(outlop[lop].tiet[i].mon, lop, (i-1)/5 + 2)){
                        VP_lop[lop][2]++;
                        fp << outlop[lop].lop << '\t' << "S2" << '\t' << outlop[lop].tiet[i].mon << '\t' << vp[2] << '\n';
                    }
                }
            }
        }
    }
    fp.close();
}

// Đếm vi phạm S3
int countBuoiDaygv(){
    int ct = 0;
    int st = 0;
    for (int gv=1; gv<=slgv; gv++){
        st = 0;
        for (int i=1; i<=30; i++){
            if (outgv[gv].tiet[i].lop != ""){
                st++;
            }
        }
        for (int i=1; i<=30; i++){
            if (outgv[gv].tiet[i].lop != ""){
                i = (((i-1)/5)+1)*5;
                ct++;
            }
        }
        ct -= (st)/5 + 1;
    }
    return ct;
}

// Đếm chi tiết S3
void VP_S3(){
    ofstream fp;
    fp.open("VP_chitiet.txt", ios::out | ios::app);
    for (int gv=1; gv<=slgv; gv++){
        int ct = 0;
        int st = 0;
        for (int i=1; i<=30; i++){
            if (outgv[gv].tiet[i].lop != ""){
                st++;
            }
        }
        for (int i=1; i<=30; i++){
            if (outgv[gv].tiet[i].lop != ""){
                i = (((i-1)/5)+1)*5;
                ct++;
            }
        }
        //cout << ct << ' ' << st << ' ';
        ct = max(ct - ((st)/5 + 1), 0);
        VP_gv[gv][3] = ct;
        if (ct > 0){
            fp << outgv[gv].gv << '\t' << "S3" << '\t' << '\t' << vp[3] << '\n';
        }
    }
    fp.close();
}

// Kiểm tra tiết k của gv i có lủng
bool TietkLungGv(int k, int i){
    if (outgv[i].tiet[k].lop == ""){ // Tiết trống
        bool c1 = false;
        bool c2 = false;
        int k_truoc = k;
        while (k_truoc >= ((k-1)/5)*5 + 1){
            if (outgv[i].tiet[k_truoc].lop != ""){ // Tiết trước đó có tiết
                c1 = true;
                break;
            }
            k_truoc--;
        }
        int k_sau = k;
        while (k_sau <= ((k-1)/5 + 1)*5){
            if (outgv[i].tiet[k_sau].lop != ""){ // Tiết sau đó có tiết
                c2 = true;
                break;
            }
            k_sau++;
        }
        if (c1 & c2){
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

// Đếm vi phạm S4
int countTietLungGv(){
    int ct = 0;
    for (int i=1; i<=slgv; i++){
        for (int thu=1; thu <= 6; thu++){
            int t = 0;
            for (int tiet = (thu-1)*5 + 1; tiet <= thu*5; tiet++){
                if (TietkLungGv(tiet, i)){
                    t++;
                }
            }
            t = max(t-1, 0);
            ct += t;
        }
    }
    return ct;
}

// Đếm chi tiết S4
void VP_S4(){
    ofstream fp;
    fp.open("VP_chitiet.txt", ios::out | ios::app);
    for (int i=1; i<=slgv; i++){
        int s = 0;
        for (int thu=1; thu <= 6; thu++){
            int t = 0;
            for (int tiet = (thu-1)*5 + 1; tiet <= thu*5; tiet++){
                if (TietkLungGv(tiet, i)){
                    t++;
                }
            }
            t = max(t-1, 0);
            s += t;
            if (t > 0){
                fp << outgv[i].gv << '\t' << "S4" << '\t' << "thu-" << thu + 1 << '\t' << vp[4] << '\n';
            }
        }
        VP_gv[i][4] = s;
    }
    fp.close();
}

// Đếm vi phạm S5
int countBuoiDay1Tiet(){
    int ct = 0;
    int st = 0;
    for (int gv=1; gv<=slgv; gv++){
        for (int thu=1; thu <= 6; thu++){
            int t = 0;
            for (int tiet = (thu-1)*5 + 1; tiet <= thu*5; tiet++){
                if (outgv[gv].tiet[tiet].lop != ""){
                    t++;
                }
            }
            if (t == 1)
            ct++;
        }
    }
    return ct;
}

// Đếm chi tiết S5
void VP_S5(){
    ofstream fp;
    fp.open("VP_chitiet.txt", ios::out | ios::app);
    for (int gv=1; gv<=slgv; gv++){
        for (int thu=1; thu <= 6; thu++){
            int t = 0;
            for (int tiet = (thu-1)*5 + 1; tiet <= thu*5; tiet++){
                if (outgv[gv].tiet[tiet].lop != ""){
                    t++;
                }
            }
            if (t == 1){
                VP_gv[gv][5]++;
                fp << outgv[gv].gv << '\t' << "S5" << '\t' << "thu-" << thu + 1 << '\t' << vp[5] << '\n';
            }
        }
    }
    fp.close();
}

// Đếm vi pham S6
int countTietToiThieu(){
    int ct = 0;
    int st = 0;
    for (int lop=1; lop<=sllop; lop++){
        for (int thu=1; thu <= 6; thu++){
            int t = 0;
            for (int tiet = (thu-1)*5 + 1; tiet <= thu*5; tiet++){
                if (lop_tiet[lop][tiet] == 1){
                    t++;
                }
            }
            if (t < 2 && t > 0)
            ct++;
        }
    }
    return ct;
}

// Đếm chi tiết S6
void VP_S6(){
    ofstream fp;
    fp.open("VP_chitiet.txt", ios::out | ios::app);
    for (int lop=1; lop<=sllop; lop++){
        for (int thu=1; thu <= 6; thu++){
            int t = 0;
            for (int tiet = (thu-1)*5 + 1; tiet <= thu*5; tiet++){
                if (lop_tiet[lop][tiet] == 1){
                    t++;
                }
            }
            if (t < 2 && t > 0){
                VP_lop[lop][6]++;
                fp << outlop[lop].lop << '\t' << "S6"  << '\t' << "thu-" << thu + 1 << '\t' << vp[6] << '\n';
            }
        }
    }
    fp.close();
}

// Đếm vi phạm S7
int countToiDaMon(){
    int ct = 0;
    int st = 0;
    for (int lop=1; lop<=sllop; lop++){
        for (int thu=1; thu <= 6; thu++){
            int t = 0;
            int tiet = (thu-1)*5 + 1;
            if (!kiemTraToiDaMon2(tiet, outlop[lop].lop)){
                ct++;
            }
        }
    }
    return ct;
}

// Đếm chi tiết S7
void VP_S7(){
    ofstream fp;
    fp.open("VP_chitiet.txt", ios::out | ios::app);
    for (int lop=1; lop<=sllop; lop++){
        for (int thu=1; thu <= 6; thu++){
            int t = 0;
            int tiet = (thu-1)*5 + 1;
            if (!kiemTraToiDaMon2(tiet, outlop[lop].lop)){
                VP_lop[lop][7]++;
                fp << outlop[lop].lop << '\t' << "S7" << '\t' << "thu-" << thu + 1 << '\t' << vp[7] << '\n';
            }
        }
    }
    fp.close();
}

// Kiêm tra tiết liên tiếp
bool kiemtratietlientiep(){
    for (int lop=1; lop<=sllop; lop++){
        for (int t=1; t<=30; t++){
            if (outlop[lop].tiet[t].mon != "" && soTiet(lop, t) > 2){
                cout << outlop[lop].lop << ' ' << t << '\n';
                return false;
            }
        }
    }
    return true;
}

// Kiểm tra giờ liên tiếp
bool kiemtratietlientietlop(int lop){
    for (int t=1; t<=30; t++){
        if (outlop[lop].tiet[t].mon != "" && soTiet(lop, t) > 2){
            return false;
        }
    }
    return true;
}

bool kiemTraTietTheDuc(){
    for (int lop=1; lop<=30; lop++){
        for (int t=5; t<=30; t+=5){
            if (outlop[lop].tiet[t].mon == "TD"){
                return false;
            }
        }
    }
    return true;
}

// Kiểm tra đụng độ phòng Tin
bool kiemTraDungDoPhongHoc2(){
    for (int tietbatdau = 1; tietbatdau <= 30; tietbatdau++){
        int count = 0;
        for (int j=1; j<=sllop; j++){
            if (lop_tiet[j][tietbatdau] == 1){
                if (outlop[j].tiet[tietbatdau].mon == "Tin"){
                    count++;
                    if (count > slphongtin){
                        cout << tietbatdau << ' ';
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

// Tính Điểm Vi Phạm (Tổng)
int TinhDiemViPham(){
    int h1 = 0;
    int h2 = 0;
    int h3 = 0;
    int h4 = 0;
    int h5 = countTietLung();
    int h6 = 0;
    int h7 = 0;
    int s1 = 0;
    int s2 = countHocCachMon();
    int s3 = countBuoiDaygv();
    int s4 = countTietLungGv();
    int s5 = countBuoiDay1Tiet();
    int s6 = countTietToiThieu();
    int s7 = countToiDaMon();
    //cout << "H1: " << h1 << '\n';
    //cout << "H2: " << h2 << '\n';
    //cout << "H3: " << h3 << '\n';
    //cout << "H4: " << h4 << '\n';
    //cout << "H5: " << h5 << '\n';
    //cout << "H6: " << h6 << '\n';
    //cout << "H7: " << h7 << '\n';

    //cout << "S1: " << s1 << '\n';
    //cout << "S2: " << s2 << '\n';
    //cout << "S3: " << s3 << '\n';
    //cout << "S4: " << s4 << '\n';
    //cout << "S5: " << s5 << '\n';
    //cout << "S6: " << s6 << '\n';
    //cout << "S7: " << s7 << '\n';
    //cout << "Điểm Vi Phạm: " << 999 * h1 + 999 * h2 + 999 * h3 + 600 * h4 + 600 * h5 + 600 * h6 + 600 * h7 + 20 * s1 + 20 * s2 + 10 * s3 + 5 * s4 + 10 * s5 + 20 * s6 + 10 * s7 << '\n';
    return 999 * h1 + 999 * h2 + 999 * h3 + 600 * h4 + 600 * h5 + 600 * h6 + 600 * h7 + vp[1] * s1 + vp[2] * s2 + vp[3] * s3 + vp[4] * s4 + vp[5] * s5 + vp[6] * s6 + vp[7] * s7;
}

// Tính Điểm Vi Phạm (Tồng) Xuất file
void TinhDiemViPham_out(){
    ofstream f_vp("VP_tong.txt");
    int h1 = 0;
    int h2 = 0;
    int h3 = 0;
    int h4 = 0;
    int h5 = countTietLung();
    int h6 = 0;
    int h7 = 0;
    int s1 = 0;
    int s2 = countHocCachMon();
    int s3 = countBuoiDaygv();
    int s4 = countTietLungGv();
    int s5 = countBuoiDay1Tiet();
    int s6 = countTietToiThieu();
    int s7 = countToiDaMon();
    f_vp << "H1: " << h1 << '\n';
    f_vp << "H2: " << h2 << '\n';
    f_vp << "H3: " << h3 << '\n';
    f_vp << "H4: " << h4 << '\n';
    f_vp << "H5: " << h5 << '\n';
    f_vp << "H6: " << h6 << '\n';
    f_vp << "H7: " << h7 << '\n';

    f_vp << "S1: " << s1 << '\n';
    f_vp << "S2: " << s2 << '\n';
    f_vp << "S3: " << s3 << '\n';
    f_vp << "S4: " << s4 << '\n';
    f_vp << "S5: " << s5 << '\n';
    f_vp << "S6: " << s6 << '\n';
    f_vp << "S7: " << s7 << '\n';
    f_vp << "Điểm Vi Phạm: " << 999 * h1 + 999 * h2 + 999 * h3 + 600 * h4 + 600 * h5 + 600 * h6 + 600 * h7 + vp[1] * s1 + vp[2] * s2 + vp[3] * s3 + vp[4] * s4 + vp[5] * s5 + vp[6] * s6 + vp[7] * s7 << '\n';
    f_vp.close();
}

// Tạo bảng vi phạm chi tiết (S)
void create_VP(){
    VP_S2();
    VP_S3();
    VP_S4();
    VP_S5();
    VP_S6();
    VP_S7();
    ofstream f_lop("VP_lop.txt");
    ofstream f_gv("VP_gv.txt");
    f_lop << "lop\tS1\tS2\tS3\tS4\tS5\tS6\tS7\n";
    for (int lop=1; lop<=sllop; lop++){
        f_lop << outlop[lop].lop << '\t';
        for (int vp=1; vp<=7; vp++){
            f_lop << VP_lop[lop][vp] << '\t';
        }
        f_lop << '\n';
    }
    f_gv << "gv\tS1\tS2\tS3\tS4\tS5\tS6\tS7\n";
    for (int gv=1; gv <= slgv; gv++){
        f_gv << outgv[gv].gv << '\t';
        for (int vp=1; vp<=7; vp++){
            f_gv << VP_gv[gv][vp] << '\t';
        }
        f_gv << '\n';
    }
    f_lop.close();
    f_gv.close();
}

// Tính Tổng số lớp của gv dạy của 1 lớp
int tongSoLopGVDayCuaMotLop(string lop){
    set <string> GV;
    for (int i=1; i<=slphancong; i++){
        if (pc[i].lop == lop){
            GV.insert(pc[i].gv);
        }
    }
    int res_t = 0;
    for (auto e : GV){
        set <string> Lop;
        for (int i=1; i<=slphancong; i++){
            if (pc[i].gv == e){
                Lop.insert(pc[i].lop);
            }
        }
        res_t += Lop.size();
    }
    return res_t;
}

// So Sách tổng số lớp
bool ComparetongSoLop(string lopa, string lopb){
    if (tongSoLopGVDayCuaMotLop(lopa) > tongSoLopGVDayCuaMotLop(lopb)){
        return true;
    }
    else{
        return false;
    }
}

// Sắp Xếp Danh Sách Lớp
void SortDanhSachLop(){
    sort(DSlop, DSlop + sllop, ComparetongSoLop);
}

// Xếp Lớp
void Xeplop(){
    for (int i=0; i<sllop; i++){
        //cout << DSlop[i] << '\n';
        Xepmotlop(DSlop[i]);
    }
}

// Số tiết liên tiếp từ tiết tiet
int soTiet(int lop, int tiet){
    if (tiet%5 == 1){
        int i = tiet;
            while (outlop[lop].tiet[tiet].mon == outlop[lop].tiet[i].mon){
                i++;
            }
        return i - tiet;
    }
    else{
        if (outlop[lop].tiet[tiet].mon != outlop[lop].tiet[tiet-1].mon){
            int i = tiet;
            while (outlop[lop].tiet[tiet].mon == outlop[lop].tiet[i].mon && i <= ((tiet-1)/5+1)*5){
                i++;
            }
            return i - tiet;
        }
        else{
            return 0;
        }
    }
}

// Đẩy Tiết Cuối lên (Làm gọn TKB)
void DayTiet(){
    for (int i=1; i<=sllop; i++){
        for (int tiet=30; tiet>=26; tiet--){
            if (lop_tiet[i][tiet] == 1){ // Có tiết
                bool ck = true;
                //  Tìm tiết trống thay thế
                for (int j=26; j>=1; j--){
                    if (lop_tiet[i][j] == 0){ // Tiết trống
                        if (ChecktietThayThe(i, j, tiet)){
                            ThayTheTiet(i, j, tiet);
                            ck = false;
                            break;
                        }
                    }
                }
                if (ck){
                    break;
                }
            }
        }
    }
}

// Kiểm tra môn có tiết cặp
bool kiemTraMon(string mon, int tiet, int lop){
    if (mon != "Van" && mon != "TD" && mon != "Tin" && mon != "Toan" && mon != "Anh"){
        return true;
    }
    else{
        if (check1tiet(lop, tiet)){
            return true;
        }
        else{
            return false;
        }
    }
}

// Nhập TKB gv (test)
void Nhap_output(){
    ifstream fi("TKB_gv.txt");
    string a;
    fi >> a;
    fi >> a;
    for (int i=1; i<=slgv; i++){
        fi >> outgv[i].gv;
    }
    for (int i=1; i<=30; i++){
        fi >> a >> a;
        for (int gv=1; gv<=slgv; gv++){
            fi >> a;
            if (a != "-"){
                outgv[gv].tiet[i].mon = a;
                fi >> a;
                fi >> outgv[gv].tiet[i].lop;
            }
        }
    }
    outgv_hetgv();
}

void Nhap_output_2(){
    ifstream fi("TKB_gv.txt");
    string a;
    fi >> a;
    fi >> a;
    for (int i=1; i<=slgv; i++){
        fi >> outgv[i].gv;
    }
    for (int i=1; i<=30; i++){
        fi >> a >> a;
        for (int gv=1; gv<=slgv; gv++){
            fi >> a;
            if (a != "-"){
                outgv[gv].tiet[i].mon = a;
                fi >> a;
                fi >> outgv[gv].tiet[i].lop;
            }
        }
    }
    outgv_hetgv();
}

void CaiThien2(){
    srand(time(NULL));
    int num = 0;
    int diem_2 = TinhDiemViPham();
    ofstream fstat;
    fstat.open("stat.txt", ios::out | ios::app);
    clock_t start;
    double time_use;
    start = clock();
    while (num < 400000){
        int lop = rand()%sllop + 1;;
        int tiet_1 = rand()%29 + 2;
        int tiet_2 = rand()%29 + 2;

        int diem = diem_2;
        if (tiet_1 != tiet_2 && lop_tiet[lop][tiet_1] == 1 && lop_tiet[lop][tiet_2] == 1){
            if (soTiet(lop, tiet_1) != 0 && soTiet(lop, tiet_1) == soTiet(lop, tiet_2)){
                int sotiet = soTiet(lop, tiet_2);
                if (sotiet == 1){
                    if (kiemTraMon(outlop[lop].tiet[tiet_1].mon, tiet_1, lop) && kiemTraMon(outlop[lop].tiet[tiet_2].mon, tiet_2, lop)){
                        if (ChecktietThayThe(lop, tiet_1, tiet_2) && ChecktietThayThe(lop, tiet_2, tiet_1)){
                            ThayTheTiet_CaiTien2(lop, tiet_1, tiet_2);
                            if (kiemtratietlientietlop(lop)){
                                diem_2 = TinhDiemViPham();
                                if (diem_2 > diem){
                                    ThayTheTiet_CaiTien2(lop, tiet_1, tiet_2);
                                    diem_2 = diem;
                                }
                                else if (diem_2 < diem){
                                    cout << "Điểm Vi Phạm: " << diem_2 << '\n';
                                    DayTiet();
                                    //ThemPCSau();
                                    num = 0;
                                    fstat << diem << '\t' << time_now + ((double)(clock() - start) / CLOCKS_PER_SEC)*1000 << '\n';
                                }
                            }
                            else{
                                cout << 1 << '\n';
                                ThayTheTiet_CaiTien2(lop, tiet_1, tiet_2);
                            }
                        }
                    }
                }
                else if (sotiet == 2){
                    if (ChecktietThayThe(lop, tiet_1, tiet_2) && ChecktietThayThe(lop, tiet_2, tiet_1) && ChecktietThayThe(lop, tiet_1 + 1, tiet_2 + 1) && ChecktietThayThe(lop, tiet_2 + 1, tiet_1 + 1)){
                        ThayTheTiet_CaiTien2(lop, tiet_1, tiet_2);
                        ThayTheTiet_CaiTien2(lop, tiet_1 + 1, tiet_2 + 1);
                        if (kiemtratietlientietlop(lop)){
                            diem_2 = TinhDiemViPham();
                            if (diem_2 > diem){
                                ThayTheTiet_CaiTien2(lop, tiet_1, tiet_2);
                                ThayTheTiet_CaiTien2(lop, tiet_1 + 1, tiet_2 + 1);
                                diem_2 = diem;
                            }
                            else if (diem_2 < diem){
                                cout << "Điểm Vi Phạm: " << diem_2 << '\n';
                                DayTiet();
                                //ThemPCSau();
                                num = 0;
                                fstat << diem << '\t' << time_now + ((double)(clock() - start) / CLOCKS_PER_SEC)*1000 << '\n';
                            }
                        }
                        else{
                            cout << 1 << '\n';
                            ThayTheTiet_CaiTien2(lop, tiet_1, tiet_2);
                            ThayTheTiet_CaiTien2(lop, tiet_1 + 1, tiet_2 + 1);
                        }
                    }
                }
            }
            else{
                if (kiemTraMon(outlop[lop].tiet[tiet_1].mon, tiet_1, lop) && kiemTraMon(outlop[lop].tiet[tiet_2].mon, tiet_2, lop) && outlop[lop].tiet[tiet_1].mon != "QP" && outlop[lop].tiet[tiet_2].mon != "QP"){
                    if (ChecktietThayThe(lop, tiet_1, tiet_2) && ChecktietThayThe(lop, tiet_2, tiet_1)){
                        ThayTheTiet_CaiTien2(lop, tiet_1, tiet_2);
                        if (kiemtratietlientietlop(lop)){
                            diem_2 = TinhDiemViPham();
                            if (diem_2 > diem){
                                ThayTheTiet_CaiTien2(lop, tiet_1, tiet_2);
                                diem_2 = diem;
                            }
                            else if (diem_2 < diem){
                                cout << "Điểm Vi Phạm: " << diem_2 << '\n';
                                DayTiet();
                                //ThemPCSau();
                                num = 0;
                                fstat << diem << '\t' << time_now + ((double)(clock() - start) / CLOCKS_PER_SEC)*1000 << '\n';
                            }
                        }
                        else{
                            cout << 1 << '\n';
                            ThayTheTiet_CaiTien2(lop, tiet_1, tiet_2);
                        }
                    }
                }
            }

        }
        num++;
    }
    fstat << TinhDiemViPham() << '\t' << time_now + ((double)(clock() - start) / CLOCKS_PER_SEC)*1000 << '\n';
    time_now += fabs(((double)(clock() - start) / CLOCKS_PER_SEC)*1000);
    fstat.close();
}


int main(int argc, char *argv[]){
    // Bắt đầu đếm thời gian
    clock_t start, end;
    double time_use;
    start = clock();

    // clear file
    ofstream ofs("stat.txt");
    ofs.close();
    ofstream ofk("VP_chitiet.txt");
    ofk.close();

    //freopen("output.txt", "w", stdout);
    memset(lop_tiet, 0, sizeof(lop_tiet));
    memset(gv_tiet, 0, sizeof(gv_tiet));

    nhap();
    ghiMucUuTien();
    sapXep();
    taoDanhSachLop();
    taoDanhSachGiaoVien();
    SortDanhSachLop();
    Xeplop();
    CaiThien1();
    CaiThien2();

    int count_loop = 0;
    // Nếu cải thiện 1 + cải thiện 2 vẫn chưa xếp hết pc thì lặp lại cải thiện 1 + cải thiện 2 (Max: 10)
    while (!checkTatCaPhanCong() && count_loop <= 10){
        cout << "===== Loop ===========\n";
        CaiThien1();
        CaiThien2();
        count_loop++;
    }

    // Xuất TKB lớp & gv
    outlop_hetlop();
    outgv_hetgv();

    cerr << checkTatCaPhanCong();
    cerr << checkTietLung();
    cerr << kiemTraDungDoPhongHoc2();
    cerr << kiemtratietlientiep();
    cerr << kiemTraTietTheDuc();

    // Tạo bảng chi tiết vi phạm
    create_VP();
    cout << "----- " << TinhDiemViPham() << " -------";
    // Tạo bảng tổng vi phạm
    TinhDiemViPham_out();


    end = clock();
    time_use = (double)(end - start) / CLOCKS_PER_SEC;
    cout << "[ ] Time: " << time_use << '\n';
    //Nhap_output();
    //xuat_test();

    /**/

}
