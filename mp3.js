async function buildMp3(audioBuffer, bitrate, vbr) {
  
  message.innerText = "Browser Storage Writing";
  if(audioBuffer.numberOfChannels==1){
    writeFile("audioBufferL", audioBuffer.getChannelData(0));
  } else {
    writeFile("audioBufferL", audioBuffer.getChannelData(0));
    writeFile("audioBufferR", audioBuffer.getChannelData(1));
  }
  message.innerText = "Start Encode";
  const ret = await Instance.encode(
                      audioBuffer.sampleRate, 
                      audioBuffer.numberOfChannels, 
                      bitrate, vbr, audioBuffer.length);
  console.log("Result: ", ret);
  message.innerText = "Complete";
  if (ret > 0){
    return Module.FS.readFile("output.mp3");
  }
  return;
}
function writeFile(name,x){
  Module.FS.writeFile(name, new Uint8Array(x.buffer));
}
