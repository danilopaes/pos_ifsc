import { useEffect, useState } from 'react';
import axios from 'axios';

export default function CommentSection({ messageId }) {
  const [comment, setComment] = useState('');
  const [comments, setComments] = useState([]);
  const token = localStorage.getItem('token');

  const fetchComments = async () => {
    const response = await axios.get(`/api/messages/${messageId}/comments`, {
      headers: { Authorization: `Bearer ${token}` },
    });
    setComments(response.data);
  };

  const submitComment = async () => {
    if (comment.trim() !== '') {
      await axios.post(`/api/messages/${messageId}/comments`, { text: comment }, {
        headers: { Authorization: `Bearer ${token}` },
      });
      setComment('');
      fetchComments();
    }
  };

  useEffect(() => {
    fetchComments();
  }, []);

  return (
    <div style={{ marginLeft: '20px' }}>
      <input value={comment} onChange={(e) => setComment(e.target.value)} placeholder="Comente" />
      <button onClick={submitComment}>Comentar</button>
      <ul>
        {comments.map((c, index) => (
          <li key={index}>{c.text}</li>
        ))}
      </ul>
    </div>
  );
}