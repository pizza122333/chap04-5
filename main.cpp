#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// 마우스 상태와 트랙바 위치를 관리하기 위한 구조체
struct GuiData {
    Mat img;
    Point ptOld;
    int track_pos;
    bool is_drawing;
};

void on_mouse(int event, int x, int y, int flags, void* userdata);
void on_trackbar(int pos, void* userdata);

int main(void)
{
    // ① 초기화면 설정: 500x500, 3채널 컬러영상(검은색)
    GuiData data;
    data.img = Mat::zeros(500, 500, CV_8UC3);
    data.track_pos = 0;
    data.is_drawing = false;

    namedWindow("Event Control");

    // 트랙바 생성: 0~3 범위, 초기위치 0
    createTrackbar("Color/Eraser", "Event Control", &data.track_pos, 3, on_trackbar, &data);

    // 마우스 콜백 등록
    setMouseCallback("Event Control", on_mouse, &data);

    imshow("Event Control", data.img);

    while (true) {
        imshow("Event Control", data.img);
        if (waitKey(1) == 27) break; // ESC 누르면 종료
    }

    return 0;
}

void on_trackbar(int pos, void* userdata) {
    // 트랙바 위치가 바뀔 때마다 콘솔에 출력 (디버깅용)
    cout << "현재 모드: " << pos << endl;
}

void on_mouse(int event, int x, int y, int flags, void* userdata)
{
    GuiData* data = (GuiData*)userdata;
    Scalar color;

    // 현재 트랙바 위치에 따른 색상 설정
    if (data->track_pos == 0)      color = Scalar(255, 0, 0); // ② 파란색 (BGR)
    else if (data->track_pos == 1) color = Scalar(0, 255, 0); // ③ 초록색
    else if (data->track_pos == 2) color = Scalar(0, 0, 255); // ④ 빨간색
    else if (data->track_pos == 3) color = Scalar(0, 0, 0);   // ⑤ 지우개 (검은색)

    switch (event) {
    case EVENT_LBUTTONDOWN:
        data->ptOld = Point(x, y);
        data->is_drawing = true;
        break;

    case EVENT_LBUTTONUP:
        data->is_drawing = false;
        break;

    case EVENT_MOUSEMOVE:
        if (data->is_drawing) {
            // 드래그하는 영역에 사각형 그리기
            // ⑤번 지우개 모드일 때는 두께를 -1(채우기)로 하면 더 잘 지워집니다.
            int thickness = (data->track_pos == 3) ? -1 : 2;

            // 실시간으로 그려지는 것을 보기 위해 rectangle 함수 호출
            rectangle(data->img, Rect(data->ptOld, Point(x, y)), color, thickness);
            data->ptOld = Point(x, y); // 연속적인 선처럼 그리려면 좌표 업데이트
            imshow("Event Control", data->img);
        }
        break;
    }
}