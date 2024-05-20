(defun my-font-lock-keywords ()
  "Add custom keywords for font-lock mode."
  (font-lock-add-keywords nil
   '(("\\<\\(TODO\\)" 1 'font-lock-warning-face t)
     ;; Define important-face with yellow foreground color and underline
     ("\\<\\(IMPORTANT\\)" 1 'important-face t)
     ;; Define note-face with red foreground color
     ("\\<\\(NOTE\\)" 1 'note-face t))))

;; Define important-face
(defface important-face
  '((t (:foreground "yellow" :underline t)))
  "Face for highlighting IMPORTANT keyword.")

;; Define note-face
(defface note-face
  '((t (:foreground "red")))
  "Face for highlighting NOTE keyword.")

(defun my-setup ()
  "Custom setup for my Emacs environment."
  ;; Load tango-dark theme
  (load-theme 'tango-dark t)
  ;; Enable display-line-numbers-mode
  (display-line-numbers-mode t)
  ;; Add custom font lock keywords
  (my-font-lock-keywords))

;; Add hooks for programming and text modes
(add-hook 'prog-mode-hook 'my-setup)
(add-hook 'text-mode-hook 'my-setup)

;; Apply theme and display-line-numbers-mode globally
(load-theme 'tango-dark t)
(global-display-line-numbers-mode t)


