# Term Count Scoring Module – Design, Implementation, and Testing Overview

## 1. Purpose and Overview

The **Term Count Scoring** module is part of the *Question Complexity Scoring* system.  
It provides a lightweight, explainable method to estimate a query’s **complexity** based on linguistic features, before it is routed to different RAG or RAGGym handlers.

This method focuses on counting the **unique meaningful terms** in a user query, reflecting how “dense” or information-heavy the question is.  
The higher the diversity of terms and entities, the higher the perceived complexity.

---

## 2. Core Design and Logic

### 🧠 NLP Processing Pipeline
The module uses **spaCy (`en_core_web_sm`)** for text processing:
1. **Tokenization & Lemmatization** – Normalizes words to their base forms.
2. **Stopword Removal** – Removes functional words like *the*, *is*, *at*, etc.
3. **Named Entity Recognition (NER)** – Captures domain-specific entities (e.g., *CAA-2*, *LKR 109 million*).

### Scoring Formula
After filtering and extracting all unique tokens and named entities:
```
score = min(1.0, round(math.log1p(unique_terms) / math.log1p(50), 3))
```
- `log1p` smooths the growth curve to prevent long questions from maxing out the score too quickly.
- The score is **normalized between 0 and 1**.

| Unique Terms | Approx. Score |
|---------------|---------------|
| 5             | 0.28          |
| 15            | 0.67          |
| 40+           | 0.98          |

---

## 3. Optional LLM-Based Refinement

For deeper semantic accuracy, the function can optionally rephrase the question using **OpenAI’s GPT-4o-mini** model.

**Purpose:**  
To simplify or clarify the query while retaining meaning, ensuring NLP analysis works on a clean version of the text.

**Implementation Flag:**  
```python
calculate_term_count_score(question, use_llm_refinement=True)
```

If refinement is enabled, the question is passed to:
```python
client.chat.completions.create(
    model="gpt-4o-mini",
    messages=[
        {"role": "system", "content": "You are an expert question simplifier."},
        {"role": "user", "content": question}
    ]
)
```

If OpenAI is unavailable, the system prints a warning and safely continues with raw input.

---

## 4. Testing Overview

### Unit Tests – `tests/test_term_count_scoring.py`
Covers:
- Basic single-term question
- Empty string input
- Comparison between diverse and simple queries

### Integration Tests – `tests/integration_test_term_count_scoring.py`
Simulates real-world questions and checks scoring consistency:
- **Simple query:** “When is the 1st payment?” → ~0.28
- **Medium query:** “Evaluate whether withholding payments…” → ~0.67
- **Complex query:** Long contractual scenario → ~0.98
- **LLM refinement test:** Ensures OpenAI refinement returns valid normalized score

---

## 5. Setup & Installation Notes

### Installation
```bash
poetry add spacy
poetry add openai
python -m spacy download en_core_web_sm
```

### Environment Variables
A valid OpenAI key must be stored in `.env`:
```
OPENAI_API_KEY=sk-...
```

### Running Tests
```bash
PYTHONPATH=$(pwd)/src poetry run pytest -sv tests/term_count_scoring.py
PYTHONPATH=$(pwd)/src poetry run pytest -sv tests/integration_test_term_count_scoring.py
```

---

## 6. Design Decisions & Deferred Ideas

### Implemented
- Tokenization + Stopword removal via spaCy
- Named entity extraction
- Logarithmic normalization
- Optional GPT-based refinement

### Skipped (For Future Work)
- NLTK-based token comparison (redundant with spaCy)
- Weighted term scoring based on POS importance
- LLM embedding-based complexity detection

---

## 7. Example Output

```bash
Q: When is the 1st payment?
 -> Score: 0.279

Q: Evaluate whether withholding payments under CAA-2 due to lack of a signed amendment is justified.
[LLM-REFINED QUESTION]: Is the client's withholding of payments due to missing CAA-2 justifiable?
 -> Score: 0.632
```

---

## 8. Summary

**Term Count Scoring** effectively bridges low-cost NLP analysis with optional LLM intelligence.  
It is fast, interpretable, and modular — suitable for routing user queries to appropriate processing pipelines based on linguistic complexity.
