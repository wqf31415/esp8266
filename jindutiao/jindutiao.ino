
#include <Wire.h>
#include <ACROBOTIC_SSD1306.h>

// 绘制进度条

unsigned char kuang[]  ={};

int length = 83;

void setup() {
  Wire.begin();  
  oled.init();                      // 初始化
  oled.clearDisplay();              // 清除屏幕
}

void loop() {
  drawKuang();
  unsigned char cc[8] = {0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81};
  for(int i=0;i<length;i++){
    int col = i/8;
    int arrlen = i%8;
    if(isLast(col)){
      int lastcount = length%8;
      if(lastcount == 0){
        cc[7]=0xff;
      }else{
        arrlen = lastcount-1;
        cc[lastcount] = 0xff;
        for(int l = lastcount+1;l<8;l++){
          cc[l] = 0x00;
        }
      }
    }
    oled.setTextXY(0,col);
    
    for(int j=0;j<arrlen+1;j++){
      if((col == 0 && j == 0)||(isLast(col) && j == arrlen)){
        cc[j] = 0xff;
      }else{
        if(j%2==1){
          cc[j]=0xab;
        }else{
          cc[j]=0xd5;
        }
      }
    }
    oled.drawBitmap(cc,8);
    oled.setTextXY(0,13);
    int k = i*100/length;
    String s = "";
    s += k;
    s= s + "%";
    oled.putString(s);
    
    delay(30);
    for(int j = 0;j<8;j++){
      cc[j] = 0x81;
    }
  } 
  oled.clearDisplay();
}

boolean isLast(int col){
  return col == (length%8 == 0? length/8-1:length/8);  
}

void drawKuang(){
  unsigned char kuang[length];
  kuang[0] = 0xff;
  for(int i=1;i<length-1;i++){
    kuang[i]=0x81;
  }
  kuang[length-1]=0xff;
  oled.setTextXY(0,0);
  oled.drawBitmap(kuang,length);
}
