BELOW ARE ALL THE REQUIREMENTS AND PREPARATIONS FOR THE DATABASE IN ORDER TO USE THIS PROJECT'S CODE. MAKE SURE THAT YOU HAVE MYSQL INSTALLED ON YOUR PC. YOU CAN USE EITHER VSCODE OR MYSQL WORKBENCH TO RUN THE FOLLOWING QUERIES.


    ------ This Project is an Illustration of HOW I UNDERSTAND Chat Applications.
    ------ I am using C++ along with Qt for the GUI implementation which will allow the app to be cross-platform.
    ------ Within Qt, I am mainly using QtCore, QtWidgets classes which encapsulates QWebSocket and QWebSocketServer for the Network/Web communication part. I Used QtMultimedia for recording audio files and the like.
    ------ All Data serialization (files, voice notes, text messages) are operated using QDataStream (another QtWidgets class).
    ------ The app uses an hybrid architecture. It uses both the Server--clients and Peer-to-Peer (P2P) Architecture for large (refers to the message's size in byte) voice notes and files.
    ------ Clients'name are dynamic within the interface i.e changing your default name will result in changing it for whoever added your phone_number.
    ------ Clients are bestowed minimal access to the Database for Security's sake. The server handles quasi every Database related query.
    ------ Swipe left to go back Implementation by overriding these 2 functions: mousePressEvent and mouseMoveEvent.
    ------ Voice Notes Feature implemented using QMediaRecorder (class within QtMultimedia).
    ------ On/Offline Status implemented using a green and red dot. 
    ------ Using WebAssembly, it's both a Desktop and Web App.
    ------ Store Data using emscripten's IDBFS, then sync it with indexed_DB to make Data consistent. File and Audio retrieval can be done without requesting it from the server (Request is done only if the indexedDB was deleted/corrupted).
    ------ Add People to Your friend_list using their phone_number.
    ------ Group Chats (Adding people from your friend_list). 
    ------ Allow only Admin to Add/Remove members within a Group.
    ------ Display Group Members by clicking on the Group's Name. When Clicking on one of them, You'll be forwarded to that person's conversation if in your friend_list, otherwise Add them to it. 
    ------ Read/Unread Messages Notification by making use of the TIMESTAMPS and the Database
    ------
    ------

    TO DO
    --- Make the GUI more appealing.
    --- Upload Preview here once Finished.
