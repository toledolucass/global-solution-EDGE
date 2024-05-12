#  Smartwatch para Monitoramento do Sono - Projeto DreamGuard

## Desafio Abordado ❗
A interseção entre saúde, inovação e tecnologia delineia um cenário de significativas transformações. Em meio à era digital, a saúde se destaca como um pilar crucial para a qualidade de vida, e a busca por soluções inovadoras promete redefinir a abordagem contemporânea a essa área fundamental.

Nessa jornada de transformação, a inovação e a tecnologia não apenas prometem aprimorar a prevenção, mas também vislumbram redefinir processos médicos, diagnósticos e tratamentos. Ao alinhar-se à visão de uma saúde mais proativa e centrada no paciente, essas ferramentas emergentes desencadeiam um ciclo virtuoso de avanços, moldando um futuro onde a assistência médica é mais acessível, precisa e integrada ao cotidiano das pessoas.

Nesse contexto, há um desafio premente que reside na necessidade de reimaginar a abordagem tradicional à saúde. A crescente interseção entre inovação e tecnologia oferece oportunidades inexploradas para moldar o futuro da prevenção de doenças, automatização de processos médicos e aprimoramento da precisão nos diagnósticos e tratamentos. Diante desse panorama, o desafio enfrentado é aproveitar plenamente o potencial dessas ferramentas emergentes, integrando-as de maneira eficaz para promover uma inovação na saúde.



## Visão Geral da Solução Proposta ⌚
O projeto "DreamGuard" propõe um smartwatch equipado com um MPU6050, contendo acelerômetro e giroscópio, para monitorar o sono do usuário. O monitoramento do sono desempenha um papel crucial na promoção da saúde, oferecendo uma compreensão detalhada dos padrões de descanso. Ao analisar fatores como a duração do sono, eficiência e interrupções, essa prática possibilita melhorias significativas na qualidade do repouso. Além de contribuir para a identificação precoce de distúrbios do sono, como apneia e insônia, essa abordagem abrangente impacta positivamente a saúde mental, reduzindo o risco de estresse e depressão. Ao personalizar hábitos saudáveis com base em dados precisos, o monitoramento do sono possibilita a melhoria da qualidade de vida de maneira holística. A solução utiliza tecnologias como Wi-Fi e MQTT (Message Queuing Telemetry Transport) para fornecer informações em tempo real sobre os padrões de sono. O dispositivo é capaz de enviar dados sobre os movimentos e a posição do corpo do usuário durante o sono para análise.

## Funcionalidades Chave do MPU6050 🔄📏

**Giroscópio:**
O giroscópio integrado no MPU6050 permite a determinação da orientação do corpo do usuário durante o sono. Isso significa que o dispositivo pode identificar se o usuário está deitado de costas, de lado ou de bruços, fornecendo insights valiosos sobre a postura adotada durante diferentes fases do sono.

**Acelerômetro:**
O acelerômetro, por sua vez, é fundamental para detectar movimentos específicos. Seja virando na cama, mudando de posição ou até mesmo levantando-se, o acelerômetro fornece dados precisos sobre esses movimentos. Isso possibilita a correlação entre os padrões de movimento e as diferentes fases do sono, como sono leve, profundo ou REM.


## Configuração e Execução da Aplicação ⚙️
**Requisitos Prévios**
- Hardware: ESP32, MPU6050.
- Software: Arduino IDE.
- FIWARE Descomplicado para a integração com o broker MQTT.

**Configuração no Arduino IDE**
- Instale as bibliotecas necessárias (WiFi, PubSubClient, Adafruit_MPU6050).
- Abra o código-fonte (.ino) no Arduino IDE.
- Configure as variáveis editáveis no início do código, como SSID, PASSWORD, BROKER_MQTT, etc., de acordo com sua rede e requisitos do projeto.

**Configuração no FIWARE Descomplicado**
- Crie um projeto no FIWARE Descomplicado.
- Adicione um dispositivo MQTT e anote o IP do Broker MQTT gerado.

**Execução**
- Conecte o ESP32 ao computador via USB.
- Carregue o código no ESP32 usando o Arduino IDE.
- Abra o monitor serial para visualizar o status da conexão Wi-Fi e MQTT.
- Conecte o smartwatch à rede Wi-Fi.
- Monitore os dados do sono no tópico MQTT especificado.

**Simulação do Projeto:**
Você pode visualizar a simulação do projeto no Wokwi, clicando <a href="https://wokwi.com/projects/381404395344200705" target="blank">aqui</a>.


Esperamos que este projeto contribua para uma abordagem inovadora na melhoria da qualidade do sono, promovendo a saúde e bem-estar geral dos usuários.