from flask import *
from openai import *
import os

app = Flask(__name__,
    static_url_path='',
)
client = OpenAI(api_key=os.getenv("OPENAI_API_KEY"))

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
            {"role": "user", "content": text}
        ]
    )
    
    output = response.choices[0].message.content
    return jsonify({"output": output})

if __name__ == "__main__":
    app.run(port=int(os.environ.get("PORT", 8080)), debug=True)