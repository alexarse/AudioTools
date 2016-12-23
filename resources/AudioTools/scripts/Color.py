#-------------------------------------------------------------
# Simple example.
#-------------------------------------------------------------
import ax;

bg_color = ax.Color(0, 0, 0);

def SetWindowColor():
    w = widgets.Get("MainWindow");
    w.SetBackgroundColor(bg_color);

# Set black.
def OnBlack():
    bg_color.r = 0.0;
    bg_color.g = 0.0;
    bg_color.b = 0.0;
    SetWindowColor();

# Set red color.
def OnRed(value):
    bg_color.r = value;
    SetWindowColor();
   
# Set green color.
def OnGreen(value):
    bg_color.g = value;
    SetWindowColor();

# Set blue color.
def OnBlue(value):
    bg_color.b = value;
    SetWindowColor();




