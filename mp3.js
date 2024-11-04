let log = function(txt){
  console.log(txt)
}

async function buildMp3(audioBuffer, bitrate, vbr) {
  
  let ch=audioBuffer.numberOfChannels;
  let x;
  message.innerText = "Browser Storage Writing";
  if(ch==1){
      x = audioBuffer.getChannelData(0);
      this.writeFile(x, "audioBufferL");
  } else {
      const l=audioBuffer.getChannelData(0),r=audioBuffer.getChannelData(1);
      this.writeFile(l, "audioBufferL");
      this.writeFile(r, "audioBufferR");
  }
  const numberOfChannels = audioBuffer.numberOfChannels,
            sampleRate = audioBuffer.sampleRate,
            nsamples = audioBuffer.length;
  message.innerText = "Start Encode";
  const ret = await Instance.encode(sampleRate, numberOfChannels, bitrate, vbr, nsamples);
  console.log("Result: ", ret);
  message.innerText = "Complete";
  if (ret > 0){
    let mp3 = Module.FS.readFile("output.mp3");
    return mp3;
  }
  return;
}
async function writeFile(x, name){
  let uint8 = new Uint8Array(x.buffer);
  Module.FS.writeFile(name, uint8);
}
