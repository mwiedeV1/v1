/**
 * @file        :  editor.js
 * @version     :  1.0
 * @created     :  Apr 14, 2010, 00:00:16 PM
 * @author      :  Canberk BOLAT <canberk.bolat at gmail.com>
 * @description :  Insert selected tag to element which defined by user
 * @function    :  addtag
 * @parameters  :  elementName - which element you want to select
 *                 tag - which tag you want to add
 * @task        :  This function takes the selected text/word/char
 *                 between specified tags
**/
function addtag(elementName, tag) {
    var obj = document.getElementById(elementName);

    beforeText = obj.value.substring(0, obj.selectionStart);
    selectedText = obj.value.substring(obj.selectionStart, obj.selectionEnd);
    afterText = obj.value.substring(obj.selectionEnd, obj.value.length);

    switch(tag) {
        
        case "bold":
            tagOpen = "[b]";
            tagClose = "[/b]";

            newText = beforeText + tagOpen + selectedText + tagClose + afterText;
        break;

        case "italic":
            tagOpen = "[i]";
            tagClose = "[/i]";

            newText = beforeText + tagOpen + selectedText + tagClose + afterText;
        break;

        case "underline":
            tagOpen = "[u]";
            tagClose = "[/u]";

            newText = beforeText + tagOpen + selectedText + tagClose + afterText;
        break;

        case "url":
            var patternHTTP = /http:\/\//i;
            url = prompt("Enter URL without http://\nExample: www.example.com", "");
            
            if (url == null) {
                break;
            } else if (url.match(patternHTTP)) {
                url = url.replace("http://", "");
            }

            tagOpen = "[url=" + url + "]";
            tagClose = "[/url]";

            newText = beforeText + tagOpen + selectedText + tagClose + afterText;
        break;

        case "img":
            imgURL = prompt("Enter image URL without http://\nExample: www.example.com/image.jpg", "");

            if (imgURL == null) {
                break;
            }

            tagOpen = "[img]" + imgURL + "";
            tagClose = "[/img]";

            newText = beforeText + tagOpen + selectedText + tagClose + afterText;
        break;
    }

    obj.value = newText;
}

