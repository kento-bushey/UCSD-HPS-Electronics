import tkinter as tk
from PIL import Image, ImageTk
import cairosvg
import io

DISPLAY_WIDTH = 800
DISPLAY_HEIGHT = 600

# Convert SVG to PNG in memory
png_data = cairosvg.svg2png(url="P.svg")
image = Image.open(io.BytesIO(png_data)).convert("RGBA")

# Scale image to fit display
img_w, img_h = image.size
scale = min(DISPLAY_WIDTH / img_w, DISPLAY_HEIGHT / img_h)
new_w = int(img_w * scale)
new_h = int(img_h * scale)
image = image.resize((new_w, new_h), Image.Resampling.LANCZOS)

# White background
bg = Image.new("RGBA", (DISPLAY_WIDTH, DISPLAY_HEIGHT), (255, 255, 255, 255))
bg.paste(image, ((DISPLAY_WIDTH - new_w)//2, (DISPLAY_HEIGHT - new_h)//2), mask=image)

# List to store clicked points
clicked_points = []

def print_coords(event):
    clicked_points.append((event.x, event.y))
    # Print current points in Arduino format
    print("const float clickedPoints[][2] = {")
    for x, y in clicked_points:
        print(f"  {{{x}, {y}}},")
    print("};\n")

root = tk.Tk()
root.title("H.svg Viewer")

canvas = tk.Canvas(root, width=DISPLAY_WIDTH, height=DISPLAY_HEIGHT)
canvas.pack()
tk_image = ImageTk.PhotoImage(bg)
canvas.create_image(0, 0, anchor="nw", image=tk_image)

canvas.bind("<Button-1>", print_coords)

root.mainloop()
