import { useEffect, useState } from 'react';
import axios from 'axios';
//import CommentSection from './CommentSection';

export default function Messages() {
  const [messages, setMessages] = useState([]);
  const [newMessage, setNewMessage] = useState('');
  const token = localStorage.getItem('token');
  const url = "http://localhost:3000";

  const fetchMessages = async () => {
    const response = await axios.get(`${url}/messages`, {
      headers: { Authorization: `Bearer ${token}` },
    });
    setMessages(response.data);
  };

  const sendMessage = async () => {
    if (newMessage.trim() !== '') {
      await axios.post(`${url}/messages`, { text: newMessage }, {
        headers: { Authorization: `Bearer ${token}` },
      });
      setNewMessage('');
      fetchMessages();
    }
  };

  useEffect(() => {
    fetchMessages();
  }, []);

  return (
    <div>
      <input value={newMessage} onChange={(e) => setNewMessage(e.target.value)} placeholder="Digite sua mensagem" />
      <button onClick={sendMessage}>Enviar</button>
      <ul>
        {messages.map((msg) => (
          <li key={msg.id}>
            {msg.text}
          </li>
        ))}
      </ul>
    </div>
  );
}