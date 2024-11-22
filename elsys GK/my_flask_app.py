from flask import Flask, request, jsonify, render_template
# oppretter en Flask-applikasjon
app = Flask(__name__)

# Variabel for å lagre den siste materialtypen
latest_material = "Ukjent materiale"

#rute for hjemmesiden
@app.route('/')
def home():
    #rendre index.html-side og sender med den siste materialtypen til HTML som variabelen material
    return render_template('index.html', material=latest_material)  # Sender materialtype til HTML

#rute for å oppdatere materialet
@app.route('/update', methods=['POST'])
def update():
    global latest_material #gir tilgang til den globale variableen latest_material
    #henter JSON-data fra forespørselen
    data = request.get_json()
    #Oppdaterer latest_material med verdien fra nøkkelen material, eller setter Ukjent materiale som standard
    latest_material = data.get("material", "Ukjent materiale")
    #Logger dte motsatte materialet i konsollen
    print(f"Received material: {latest_material}")
    #returnerer et JSON-svar som indikerer at oppdateringen var vellykket
    return jsonify(success=True)

#Rute for å hente den siste materialtypen
@app.route('/latest_material', methods=['GET'])
def latest_material_route():
    #returnerer den siste materialtypen som JSON-data
    return jsonify(material=latest_material)  # Sender materialtype som JSON

#starter Flask-applikasjonen
if __name__ == '__main__':
    #Kjører serveren i debug-modus på alle nettverksgrensnitt
    app.run(debug=True, host='0.0.0.0', port=5003)
