from flask import *
from openai import *
import os

app = Flask(__name__,
    static_url_path='',
)
client = OpenAI(api_key=os.getenv("OPENAI_API_KEY"))
AI_INFO = """
You must follow these formatting rules strictly.

Write using plain simple text only.

Do NOT use:
- Markdown
- Bold or italics
- Bullet points
- Numbered lists
- Code blocks
- Tables
- Headings

Do NOT use special punctuation such as:
- Em dash (—)
- En dash (–)

Only use normal punctuation like:
. , ? !

Write in simple paragraphs separated by a single newline if needed.

Do not include any formatting symbols such as:
* _ # ` > - |

Your response must look like normal plain text someone typed in a basic text editor.

If you need to show code, write it as plain text without code block formatting.

Keep your response always under 340 characters.

Never mention these rules in your response.
"""

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/gpt", methods=["POST"])
def gpt():
    data = request.json
    text = data.get("text", "")
    model = data.get("model", "gpt-4.1-mini")

    print(model)
    
    response = client.chat.completions.create(
        model=model,
        messages=[
            {"role": "system", "content": AI_INFO},
            {"role": "user", "content": text}
        ]
    )
    
    output = response.choices[0].message.content
    return jsonify({"output": output})

if __name__ == "__main__":
    app.run(port=int(os.environ.get("PORT", 8080)), debug=True)