#include "calculator.h"
#include "../ui/graphics.h"
#include "../driver/keyboard.h"
#include <stdio.h>

static int32_t num1 = 0;
static int32_t num2 = 0;
static char op = '+';
static int8_t input_stage = 0; // 0 - ввод Num1, 1 - выбор операции, 2 - ввод Num2 / вывод результата

void UI_DisplayCalculator(void)
{
    UI_ClearScreen();
    // Красивая рамка и заголовок
    UI_PrintString("CALCULATOR", 10, 118, 1, 8, false);
    UI_DrawLine(0, 12, 127, 12, true);

    char buf[32];
    if (input_stage == 0) {
        snprintf(buf, sizeof(buf), "Num1: %d_", num1);
    } else if (input_stage == 1) {
        snprintf(buf, sizeof(buf), "%d %c", num1, op);
    } else {
        snprintf(buf, sizeof(buf), "%d %c %d", num1, op, num2);
    }
    
    // Вывод текущего математического выражения по центру экрана
    UI_PrintString(buf, 10, 100, 3, 8, false);

    // Подсказка по управлению кнопками внизу экрана
    UI_DrawLine(0, 52, 127, 52, true);
    UI_PrintString("M: NEXT/EQUAL | EXIT: BACK", 2, 120, 5, 7, false);
    UI_UpdateDisplay();
}

void APP_CalculatorProcess(void)
{
    KEY_Code_t key;
    input_stage = 0;
    num1 = 0; num2 = 0; op = '+';
    
    UI_DisplayCalculator();

    while (1) {
        key = KEY_GetPressed();
        if (key == KEY_EXIT) {
            if (input_stage > 0) { 
                input_stage--; 
                UI_DisplayCalculator(); 
            } else { 
                break; // Полный выход из калькулятора обратно в рацию
            }
        }
        
        // Обработка ввода цифр с клавиатуры рации (0-9)
        if (key >= KEY_0 && key <= KEY_9) {
            int8_t val = key - KEY_0;
            if (input_stage == 0) {
                if (num1 < 100000000) num1 = num1 * 10 + val;
            } else if (input_stage == 2) {
                if (num2 < 100000000) num2 = num2 * 10 + val;
            }
            UI_DisplayCalculator();
        }

        // Выбор математической операции стрелками вверх/вниз
        if (input_stage == 1) {
            if (key == KEY_UP) { 
                op = (op == '+') ? '-' : (op == '-') ? '*' : (op == '*') ? '/' : '+'; 
                UI_DisplayCalculator(); 
            }
            if (key == KEY_DOWN) { 
                op = (op == '+') ? '/' : (op == '/') ? '*' : (op == '*') ? '-' : '+'; 
                UI_DisplayCalculator(); 
            }
        }

        // Подтверждение этапа или подсчет результата кнопкой MENU (M)
        if (key == KEY_MENU) {
            if (input_stage == 0) {
                input_stage = 1;
            } else if (input_stage == 1) {
                input_stage = 2;
            } else if (input_stage == 2) {
                int32_t res = 0;
                if (op == '+') res = num1 + num2;
                if (op == '-') res = num1 - num2;
                if (op == '*') res = num1 * num2;
                if (op == '/') res = (num2 != 0) ? num1 / num2 : 0;
                
                num1 = res; // Результат вычисления переносим в первое число
                num2 = 0;
                input_stage = 1; // Возвращаемся к выбору следующего действия
            }
            UI_DisplayCalculator();
        }
    }
}
