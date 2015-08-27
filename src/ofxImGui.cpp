#include "ofxImGui.h"

#if 0
int ofxImGui::g_ShaderHandle = 0;
int ofxImGui::g_AttribLocationTex =0;
int ofxImGui::g_AttribLocationProjMtx=0;
unsigned int ofxImGui::g_VaoHandle=0;
unsigned int ofxImGui::g_VboHandle=0;
unsigned int ofxImGui::g_ElementsHandle = 0;
#endif

ofTexture ofxImGui::fontTexture;
ofxImGui::ofxImGui()
{
    time = 0.0f;
    mouseWheel = 0.0f;

#ifndef TARGET_OPENGLES
    ofAppGLFWWindow* baseWindow = (ofAppGLFWWindow*)ofGetWindowPtr();
    glfwWindow = baseWindow->getGLFWWindow();
#endif    
    
    io = &ImGui::GetIO();
    
    ofAddListener(ofEvents().keyPressed, this, &ofxImGui::onKeyPressed);
    ofAddListener(ofEvents().keyReleased, this, &ofxImGui::onKeyReleased);
    ofAddListener(ofEvents().mousePressed, this, &ofxImGui::onMousePressed);
    ofAddListener(ofEvents().mouseScrolled, this, &ofxImGui::onMouseScrolled);
}


void ofxImGui::setup()
{   
     //Keyboard mapping. ImGui will use those indices to peek into the io->KeyDown[] array.
    io->KeyMap[ImGuiKey_Tab] = OF_KEY_TAB;
    io->KeyMap[ImGuiKey_LeftArrow] = OF_KEY_LEFT;
    io->KeyMap[ImGuiKey_RightArrow] = OF_KEY_RIGHT;
    io->KeyMap[ImGuiKey_UpArrow] = OF_KEY_UP;
    io->KeyMap[ImGuiKey_DownArrow] = OF_KEY_DOWN;
    io->KeyMap[ImGuiKey_PageUp] = OF_KEY_PAGE_UP;
    io->KeyMap[ImGuiKey_PageDown] = OF_KEY_PAGE_DOWN;
    io->KeyMap[ImGuiKey_Home] = OF_KEY_HOME;
    io->KeyMap[ImGuiKey_End] = OF_KEY_END;
    io->KeyMap[ImGuiKey_Delete] = OF_KEY_DEL;
    io->KeyMap[ImGuiKey_Backspace] = OF_KEY_BACKSPACE;
    io->KeyMap[ImGuiKey_Enter] = OF_KEY_RETURN;
    io->KeyMap[ImGuiKey_Escape] = OF_KEY_ESC;
    io->RenderDrawListsFn =  &ofxImGui::renderDrawLists;
    io->SetClipboardTextFn = ofxImGui::setClipboardString;
    io->GetClipboardTextFn = &ofxImGui::getClipboardString;
    
}

void ofxImGui::onKeyPressed(ofKeyEventArgs& event)
{
    io->KeysDown[event.key] = true;
    
}

void ofxImGui::onKeyReleased(ofKeyEventArgs& event)
{
    
    io->KeysDown[event.key] = false;
    io->AddInputCharacter((unsigned short)event.codepoint);
    
}

void ofxImGui::onMousePressed(ofMouseEventArgs& event)
{
    if (event.button >= 0 && event.button < 3)
    {
        mousePressed[event.button] = true;
    }
}

void ofxImGui::onMouseScrolled(ofMouseEventArgs& event)
{    
    mouseWheel += (float)event.y;
}

#if 0
void ofxImGui::renderDrawLists_GLES(ImDrawData* draw_data)
{
    
  #if 0
    GLint last_program, last_texture;
    glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glActiveTexture(GL_TEXTURE0);
    
    // Setup orthographic projection matrix
    const float width = ImGui::GetIO().DisplaySize.x;
    const float height = ImGui::GetIO().DisplaySize.y;
    const float ortho_projection[4][4] =
    {
        { 2.0f/width,	0.0f,			0.0f,		0.0f },
        { 0.0f,			2.0f/-height,	0.0f,		0.0f },
        { 0.0f,			0.0f,			-1.0f,		0.0f },
        { -1.0f,		1.0f,			0.0f,		1.0f },
    };
    glUseProgram(g_ShaderHandle);
    glUniform1i(g_AttribLocationTex, 0);
    glUniformMatrix4fv(g_AttribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);
    glBindVertexArrayFunc(g_VaoHandle);
    
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const ImDrawIdx* idx_buffer_offset = 0;
        
        glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.size() * sizeof(ImDrawVert), (GLvoid*)&cmd_list->VtxBuffer.front(), GL_STREAM_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ElementsHandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx), (GLvoid*)&cmd_list->IdxBuffer.front(), GL_STREAM_DRAW);
        
        for (const ImDrawCmd* pcmd = cmd_list->CmdBuffer.begin(); pcmd != cmd_list->CmdBuffer.end(); pcmd++)
        {
            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                glScissor((int)pcmd->ClipRect.x, (int)(height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT, idx_buffer_offset);
            }
            idx_buffer_offset += pcmd->ElemCount;
        }
    }
    
    // Restore modified state
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(last_program);
    glDisable(GL_SCISSOR_TEST);
    glBindTexture(GL_TEXTURE_2D, last_texture);
    #endif
}
#endif


//https://github.com/ocornut/imgui/commit/59d498f3d0319dab32b3f4842c6e5f2da6d68830

ofFloatColor ofxImGui::convertToFloatColor(ImU32 rgba)
{
    float sc = 1.0f/255.0f;
    
    ofFloatColor result;
    result.r = (float)(rgba&0xFF) * sc; 
    result.g = (float)((rgba>>8)&0xFF) * sc; 
    result.b = (float)((rgba>>16)&0xFF) * sc; 
    result.a = (float)(rgba >> 24) * sc;
    return result;
}

//#ifndef TARGET_OPENGLES
void ofxImGui::renderDrawLists(ImDrawData* draw_data)
{
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)ofxImGui::fontTexture.texData.textureID);
    glEnable(GL_SCISSOR_TEST);
    for (int n = 0; n <  draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        
    
        
        ofVboMesh mesh;
        vector<ofVec3f> verts;
        vector<ofVec2f> texCoords;
        vector<ofFloatColor> colors;
        vector<ofIndexType> index;
        for(size_t i = 0; i<cmd_list->VtxBuffer.size(); i++)
        {
            verts.push_back(ofVec3f(cmd_list->VtxBuffer[i].pos.x, cmd_list->VtxBuffer[i].pos.y, 0));
            texCoords.push_back(ofVec2f(cmd_list->VtxBuffer[i].uv.x, cmd_list->VtxBuffer[i].uv.y));
            colors.push_back(ofxImGui::convertToFloatColor(cmd_list->VtxBuffer[i].col));
            
        }
        
        for(size_t i = 0; i<cmd_list->IdxBuffer.size(); i++)
        {
            index.push_back((ofIndexType)cmd_list->IdxBuffer[i] );
        }
        
        
        mesh.addVertices(verts);
        mesh.addTexCoords(texCoords);
        mesh.addColors(colors);
        mesh.addIndices(index);        
        

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                ofLogVerbose() << "UserCallback: ";
                
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                
                glScissor((int)pcmd->ClipRect.x, 
                          (int)(ofGetHeight() - pcmd->ClipRect.w), 
                          (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), 
                          (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));

                mesh.getVbo().drawElements(GL_TRIANGLES, index.size());
               
                
            }
        }
    }
     glBindTexture(GL_TEXTURE_2D, 0);
   



}
//#endif

const char* ofxImGui::getClipboardString()
{    
    return ofGetWindowPtr()->getClipboardString().c_str();
}

void ofxImGui::setClipboardString(const char* text)
{
    ofGetWindowPtr()->setClipboardString(text);
}


bool ofxImGui::createDeviceObjects()
{
    // Build texture
    unsigned char* pixels;
    int width, height;
    io->Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);
    
    GLuint externalTexture;
    glGenTextures(1, &externalTexture);
    glBindTexture(GL_TEXTURE_2D, externalTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 
                 0,
                 GL_ALPHA,
                 width,
                 height, 
                 0,
                 GL_ALPHA, 
                 GL_UNSIGNED_BYTE, 
                 pixels);
    ofxImGui::fontTexture.setUseExternalTextureID(externalTexture);
    
    // Store our identifier
    io->Fonts->TexID = (void *)(intptr_t)ofxImGui::fontTexture.getTextureData().textureID;


    // Cleanup (don't clear the input data if you want to append new fonts later)
    io->Fonts->ClearInputData();
    io->Fonts->ClearTexData();

    return true;
}

void ofxImGui::begin()
{
    if (!ofxImGui::fontTexture.isAllocated())
    {
         createDeviceObjects();
    }
    
    // Setup inputs
    // (we already got mouse wheel, keyboard keys & characters from glfw callbacks polled in glfwPollEvents())
    updateFrame();
    
    // Start the frame
    ImGui::NewFrame();
}

void ofxImGui::updateFrame()
{
    
    // Setup time step
    float current_time =  ofGetElapsedTimef();
    
    if(time > 0.0 )
    {
        io->DeltaTime =  current_time - time;
    }else
    {
        io->DeltaTime =1.0f/60.0f;
    }
    
    time =  current_time;
    
    // Setup display size (every frame to accommodate for window resizing)
    //int w, h;
    int w = ofGetWindowWidth();
    int h = ofGetWindowHeight();
    io->DisplaySize = ImVec2((float)w, (float)h);
    
#if USING_GLFW
    if (glfwGetWindowAttrib(glfwWindow, GLFW_FOCUSED))
    {
        double mouse_x, mouse_y;
        mouse_x = (double)ofGetMouseX();
        mouse_y = (double)ofGetMouseY();
        mouse_x *= (float)ofGetScreenWidth() / w; // Convert mouse coordinates to pixels
        mouse_y *= (float)ofGetScreenHeight() / h;
        
        // Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)
        io->MousePos = ImVec2((float)mouse_x, (float)mouse_y);   
    }
    else
    {
        io->MousePos = ImVec2(-1,-1);
    }
#else
    io->MousePos = ImVec2((float)ofGetMouseX(), (float)ofGetMouseY());   
#endif
    for (int i = 0; i < 3; i++)
    {
        /*
         If a mouse press event came, always pass it as "mouse held this frame", 
         so we don't miss click-release events that are shorter than 1 frame.
         */
        
        io->MouseDown[i] = mousePressed[i] || ofGetMousePressed(i);  
        mousePressed[i] = false;
    }
    
    io->MouseWheel = mouseWheel;
    mouseWheel = 0.0f;
    
    // Hide OS mouse cursor if ImGui is drawing it
    if(io->MouseDrawCursor)
    {
        ofHideCursor();
    }
    
}


void ofxImGui::end()
{
    ImGui::Render(); 
}

ofxImGui::~ofxImGui()
{
    ImGui::GetIO().Fonts->TexID = 0;
    io=NULL;
    ImGui::Shutdown();
}






