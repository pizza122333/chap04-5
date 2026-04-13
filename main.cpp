#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// 이미지와 트랙바 위치를 함께 전달하기 위한 구조체 (선택 사항이지만 깔끔합니다)
struct UserData {
    Mat img;
    int track_pos;
};

void on_mouse(int event, int x, int y, int flags, void* userdata);
void on_trackbar(int pos, void* userdata);

int main(void)
{
    // 원본 이미지를 불러와서 복사본을 하나 만듭니다. (그림을 그리기 위함)
    Mat src = imread("lenna.bmp");
    if (src.empty()) {
        cerr << "Image load failed!" << endl;
        return -1;
    }

    // 구조체에 이미지와 초기 위치를 저장
    UserData data;
    data.img = src.clone(); // 원본 보존을 위해 복사본 사용
    data.track_pos = 0;

    namedWindow("src");

    // 마우스 콜백: 왼쪽 클릭 시 현재 트랙바 위치 출력
    setMouseCallback("src", on_mouse, &data);

    // 트랙바 생성: 최대값 100, data 구조체 주소 전달
    createTrackbar("radius", "src", 0, 100, on_trackbar, &data);

    imshow("src", data.img);
    waitKey();
    return 0;
}

void on_mouse(int event, int x, int y, int flags, void* userdata)
{
    UserData* data = (UserData*)userdata;

    switch (event) {
    case EVENT_LBUTTONDOWN:
        // 현재 트랙바 위치(radius)를 콘솔에 출력
        cout << "현재 트랙바 위치 (원의 반지름): " << data->track_pos << endl;
        break;
    }
}

void on_trackbar(int pos, void* userdata)
{
    UserData* data = (UserData*)userdata;
    data->track_pos = pos; // 현재 위치 저장

    // 매번 원본에서 다시 시작해야 원이 겹치지 않고 하나만 보입니다.
    Mat display = data->img.clone();

    // 트랙바 위치(pos)를 반지름으로 하는 원 그리기
    // 좌표 (256, 256)에 초록색(0, 255, 0), 두께 2로 설정
    if (pos > 0) {
        circle(display, Point(256, 256), pos, Scalar(0, 255, 0), 2);
    }

    // 변경된 이미지를 화면에 즉시 업데이트
    imshow("src", display);
}