import sys
import socket
from transformers import pipeline


#load gpt model
print("Loading GPT-2 model")
chat = pipeline("text-generation", model = "gpt2")
print("Model is loaded")

HOST = "127.0.0.1"
PORT = 9090

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen(1)
    print(f"ChatBot Server Listening on {HOST}:{PORT}")

    conn = s.accept()
    addr = s.accept()

    with conn:
        print(f"connected by {addr}")
        while True:
            data = conn.recv(1024)
            if not data:
                break
            user_in = data.decode("utf-8").strip()
            if user_in == "exit":
                conn.sendall("Goodbye!")
                break
            response = chat(user_in, max_length=50, num_return_sequences=1)
            reply = response[0]["generated_text"].replace("\n", " ")
            conn.sendall(reply.encode("utf-8"))
