#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <vector>
#include <array>
#include <algorithm>
#include <windows.h>
#include "util/img.h"
#include "neural/activations.h"
#include "neural/nn.h"
#include "matrix/matrix.h"
#include "matrix/ops.h"

#define MAX_LINE_LENGTH 1024
#define ROWS 28
#define COLS 28

POINT mouseDownPt;
bool isMouseButtonDown = false;

HBITMAP canvasBufferBitmap = nullptr;
HDC canvasHDC = nullptr;

LRESULT CALLBACK main_window_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void saveAsCSV();

int windowWidth = 420;
int windowHeight = 420;

int main(int argc, char **argv)
{
    srand(time(NULL));

    // TRAINING
    // int number_imgs = 10000;
    //  Img** imgs = csv_to_imgs("./data/mnist_train.csv", number_imgs);
    //  NeuralNetwork* net = network_create(784, 300, 10, 0.1);
    //  network_train_batch_imgs(net, imgs, number_imgs);
    //  network_save(net, "testing_net");

    // PREDICTING
    int n, number_imgs = 3000;
    Img **imgs = csv_to_imgs("data/mnist_test.csv", number_imgs);

   // img_print(imgs[0]);

    NeuralNetwork *net = network_load("testing_net");
    double score = network_predict_imgs(net, imgs, 1000);
    printf("Score: %1.5f\n", score);

 

    //=======================================================================================================

     printf("How many numbers do you wanna test?\n");
        scanf("%d", &n);
        printf("\n");

        for (int i = 0; i < n; i++)
        {
            char window_class_name[50];
            sprintf(window_class_name, "main_window_class%d", i);

            WNDCLASSEX wincl;
            HWND hwnd;

            wincl.hInstance = GetModuleHandle(0);
            wincl.lpszClassName = window_class_name;
            wincl.lpfnWndProc = main_window_proc;
            wincl.style = CS_DBLCLKS;
            wincl.cbSize = sizeof(WNDCLASSEX);

            wincl.hIcon = LoadIcon(0, IDI_APPLICATION);
            wincl.hIconSm = LoadIcon(0, IDI_APPLICATION);
            wincl.hCursor = LoadCursor(0, IDC_ARROW);
            wincl.lpszMenuName = 0;
            wincl.cbClsExtra = 0;
            wincl.cbWndExtra = 0;
            wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

            RECT wr = {0, 0, windowWidth, windowHeight};       // set the size, but not the position
            AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE); // adjust the size

            if (RegisterClassEx(&wincl))
            {
                hwnd = CreateWindowEx(
                    0,
                    window_class_name,
                    "Hello Canvas!",
                    WS_OVERLAPPEDWINDOW,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    wr.right - wr.left,
                    wr.bottom - wr.top,
                    HWND_DESKTOP,
                    0,
                    wincl.hInstance,
                    0);

                RECT rcClient;
                GetClientRect(hwnd, &rcClient);

                //-----------------
                HWND hwndOkButton = CreateWindow(
                    "BUTTON", // Predefined class; Unicode assumed
                    "OK",     // Button text

                    WS_CHILD | WS_VISIBLE, // Styles
                    (rcClient.right / 2) - 50,
                    rcClient.bottom - 40,
                    100,
                    30,
                    hwnd, // Parent window
                    (HMENU)105,
                    wincl.hInstance,
                    0); // Pointer not needed.

                printf("actual canvas size %d X %d\n", rcClient.right, rcClient.bottom);

                HDC hdc = GetDC(hwnd);
                canvasBufferBitmap = CreateCompatibleBitmap(hdc, rcClient.right, rcClient.bottom);
                canvasHDC = CreateCompatibleDC(hdc);
                SelectObject(canvasHDC, canvasBufferBitmap);

                // FillRect(canvasHDC, &rcClient, (HBRUSH)GetStockObject(WHITE_BRUSH));

                ReleaseDC(hwnd, hdc);

                ShowWindow(hwnd, SW_SHOW);
            }

            //===============================================================================

            // Set a color for our drawing for which, we need to create a pen object
            HPEN hPen = CreatePen(PS_SOLID, 40, RGB(255, 255, 255));
            SelectObject(canvasHDC, hPen);
            SelectObject(canvasHDC, (HBRUSH)GetStockObject(HOLLOW_BRUSH));

            // Ellipse(canvasHDC, 20, 20, 400, 400);
            // DrawText(canvasHDC, "A", )

            // Main even loop
            MSG messages;
            while (GetMessage(&messages, 0, 0, 0))
            {
                TranslateMessage(&messages);
                DispatchMessage(&messages);
            }
            // Application Exit point
            printf("Saving as CSV...\n");
            saveAsCSV();

            // Make sure to delete all the gdi32 objects we
            // created to keep the memory clean and leak free!

            DeleteObject(hPen);

            if (canvasHDC != nullptr)
            {
                DeleteDC(canvasHDC);
            }

            if (canvasBufferBitmap != nullptr)
            {
                DeleteObject(canvasBufferBitmap);
            }
            printf("Quiting...\n");





        

    int image[ROWS][COLS];
    FILE *file = fopen("C:\\Users\\msi\\Desktop\\SPL1-Hand-written-number-detection-main\\imageOutput.txt", "r");

    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (!fscanf(file, "%d, ", &image[i][j]))
            {
                break;
            }
        }
    }

    fclose(file);

    // Print the array to verify it was read correctly
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printf("%d ", image[i][j]);
        }
        printf("\n");
    }

   

    printf("\n");

    Matrix *input_data = image_to_matrix(image);
   
     
    Img *img = (Img*)malloc(sizeof(Img));
    img->img_data = input_data;
    img->label = -1; // or whatever default value you want to use
    

    Matrix* prediction = network_predict_img(net, img);
   
   // matrix_print(prediction);

    int predicted_label = 0;
    double max_value = prediction->entries[0][0];

    for (int i = 1; i < prediction->rows; i++)
    {
        if (prediction->entries[i][0] > max_value)
        {
            max_value = prediction->entries[i][0];
            predicted_label = i;
        }
    }

    printf("The predicted label is: %d\n", predicted_label);

        }

    imgs_free(imgs, number_imgs);
    network_free(net);

    return 0;
}

//===============================================================================================================================

float cubic_hermite(float A, float B, float C, float D, float t)
{

    float a = -A / 2.0f + (3.0f * B) / 2.0f - (3.0f * C) / 2.0f + D / 2.0f;
    float b = A - (5.0f * B) / 2.0f + 2.0f * C - D / 2.0f;
    float c = -A / 2.0f + C / 2.0f;
    float d = B;

    return a * t * t * t + b * t * t + c * t + d;
}

#define CLAMP(v, min, max) \
    if (v < min)           \
    {                      \
        v = min;           \
    }                      \
    else if (v > max)      \
    {                      \
        v = max;           \
    }

const unsigned char *get_pixel_clamped(const std::vector<unsigned char> &data, int w, int h, int pitch, int x, int y)
{
    CLAMP(x, 0, w - 1);
    CLAMP(y, 0, h - 1);
    return &data[(y * pitch) + x * 4];
}

std::array<unsigned char, 4> sample_bicubic(const std::vector<unsigned char> &data, int w, int h, int pitch, float u, float v)
{

    float x = (u * w) - 0.5;
    int xint = (int)x;
    float xfract = x - floor(x);

    float y = (v * h) - 0.5;
    int yint = (int)y;
    float yfract = y - floor(y);

    int i;

    // 1st row
    auto p00 = get_pixel_clamped(data, w, h, pitch, xint - 1, yint - 1);
    auto p10 = get_pixel_clamped(data, w, h, pitch, xint + 0, yint - 1);
    auto p20 = get_pixel_clamped(data, w, h, pitch, xint + 1, yint - 1);
    auto p30 = get_pixel_clamped(data, w, h, pitch, xint + 2, yint - 1);

    // 2nd row
    auto p01 = get_pixel_clamped(data, w, h, pitch, xint - 1, yint + 0);
    auto p11 = get_pixel_clamped(data, w, h, pitch, xint + 0, yint + 0);
    auto p21 = get_pixel_clamped(data, w, h, pitch, xint + 1, yint + 0);
    auto p31 = get_pixel_clamped(data, w, h, pitch, xint + 2, yint + 0);

    // 3rd row
    auto p02 = get_pixel_clamped(data, w, h, pitch, xint - 1, yint + 1);
    auto p12 = get_pixel_clamped(data, w, h, pitch, xint + 0, yint + 1);
    auto p22 = get_pixel_clamped(data, w, h, pitch, xint + 1, yint + 1);
    auto p32 = get_pixel_clamped(data, w, h, pitch, xint + 2, yint + 1);

    // 4th row
    auto p03 = get_pixel_clamped(data, w, h, pitch, xint - 1, yint + 2);
    auto p13 = get_pixel_clamped(data, w, h, pitch, xint + 0, yint + 2);
    auto p23 = get_pixel_clamped(data, w, h, pitch, xint + 1, yint + 2);
    auto p33 = get_pixel_clamped(data, w, h, pitch, xint + 2, yint + 2);

    std::array<unsigned char, 4> ret;
    // interpolate bi-cubically!
    for (i = 0; i < 4; i++)
    {

        float col0 = cubic_hermite(p00[i], p10[i], p20[i], p30[i], xfract);
        float col1 = cubic_hermite(p01[i], p11[i], p21[i], p31[i], xfract);
        float col2 = cubic_hermite(p02[i], p12[i], p22[i], p32[i], xfract);
        float col3 = cubic_hermite(p03[i], p13[i], p23[i], p33[i], xfract);

        float value = cubic_hermite(col0, col1, col2, col3, yfract);

        CLAMP(value, 0.0f, 255.0f);

        ret[i] = (unsigned char)value;
    }
    return ret;
}

std::vector<std::vector<unsigned char>> getPixelData(int target_w, int target_h)
{

    // collect pixel data
    BITMAP bm = {0};
    size_t ret = ::GetObject(canvasBufferBitmap, sizeof(BITMAP), (LPSTR)&bm);

    BITMAPFILEHEADER bmfHeader;
    BITMAPINFOHEADER bi;

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bm.bmWidth;
    bi.biHeight = -bm.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    int pitch = ((bm.bmWidth * bi.biBitCount + 31) / 32) * 4;

    int dataSize = pitch * bm.bmHeight;

    std::vector<unsigned char> pixelData(dataSize);

    HDC dc = ::GetDC(nullptr);

    const int inputLines = bm.bmHeight;
    const int outputLines = GetDIBits(dc, canvasBufferBitmap, 0, inputLines, &pixelData[0], (BITMAPINFO *)&bi, DIB_RGB_COLORS);

    if (outputLines != inputLines)
    {
        printf("Failed to get raw data from the canvas bitmap.");
    }
    ::ReleaseDC(nullptr, dc);

    int original_w = bm.bmWidth;
    int original_h = bm.bmHeight;

    // resize the data into target size using bicubic sampler

    std::vector<std::vector<unsigned char>> result(target_w, std::vector<unsigned char>(target_h));

    for (int y = 0; y < target_h; ++y)
    {

        float v = float(y) / float(target_h - 1);

        for (int x = 0; x < target_w; ++x)
        {

            float u = float(x) / float(target_w - 1);

            std::array<unsigned char, 4> sample = sample_bicubic(pixelData, original_w, original_h, pitch, u, v);
            result[x][y] = sample[0]; // we just need one color the others don't matter
        }
    }
    return result;
}

void saveAsCSV()
{

    int width = 28;
    int height = 28;
    std::vector<std::vector<unsigned char>> data = getPixelData(width, height);

    FILE *file = fopen("C:\\Users\\msi\\Desktop\\SPL1-Hand-written-number-detection-main\\imageOutput.txt", "w");

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            unsigned char pixelValue = data[x][y];
            fprintf(file, "%d, ", pixelValue);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

LRESULT CALLBACK main_window_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    case WM_COMMAND:
    {
        int id = LOWORD(wParam);
        if (id == 105)
        {
            PostMessage(hwnd, WM_CLOSE, 0, 0);
        }
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        BeginPaint(hwnd, &ps);
        HDC hdc = ps.hdc;
        RECT rcClient;
        GetClientRect(hwnd, &rcClient);

        BitBlt(hdc, 0, 0, rcClient.right, rcClient.bottom, canvasHDC, 0, 0, SRCCOPY);

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        MoveToEx(canvasHDC, x, y, nullptr);

        mouseDownPt.x = x;
        mouseDownPt.y = y;
        isMouseButtonDown = true;

        InvalidateRect(hwnd, nullptr, true);

        break;
    }
    case WM_LBUTTONUP:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        isMouseButtonDown = false;

        InvalidateRect(hwnd, nullptr, true);

        break;
    }
    case WM_MOUSEMOVE:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        if (isMouseButtonDown)
        {
            LineTo(canvasHDC, x, y);

            InvalidateRect(hwnd, nullptr, true);
        }

        break;
    }
    case WM_SIZE:
    {
        RECT rect;
        GetClientRect(hwnd, &rect);
        break;
    }
    case WM_ERASEBKGND:
        return 1;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------

// g++ -o main matrix/*.c neural/*.c util/*.c *.cpp -lm -lgdi32
// ./main
