package com.ingenieriedavinci.leonardusui;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.OutputStream;
import java.util.UUID;

public class ListeFormes extends AppCompatActivity
{
    //Statics - Types de formes
    private static final int POLYGONES = 0;
    private static final int ELLIPSES = 1;
    private static final int SPIRALES = 2;
    private static final int PARALLELOGRAMMES = 3;
    private static final int EMOJI = 4;
    private static final int ETOILES = 5;
    private static final int AUTRES = 6;

    //Variables
    int typeFormes;

    Forme[] formes;
    ArrayAdapter<Forme> adapteurFormes;
    Telecommande telecommande;

    //Constructeurs
    public ListeFormes() {}

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_liste_formes);

        //Attributions
        typeFormes = getIntent().getExtras().getInt("typeFormes");
        telecommande = new Telecommande();

        switch(typeFormes)
        {
            case POLYGONES:
                formes = new Forme[11];//De 2 à 12 inclu
                formes[0] = new Forme("Digone","forme_polygone_digone", Forme.Difficulte.Facile);
                formes[1] = new Forme("Triangle","forme_polygone_triangle", Forme.Difficulte.Normale);
                formes[2] = new Forme("Carré","forme_polygone_carre", Forme.Difficulte.Facile);
                formes[3] = new Forme("Pentagone","forme_polygone_pentagone", Forme.Difficulte.Normale);
                formes[4] = new Forme("Hexagone","forme_polygone_hexagone", Forme.Difficulte.Normale);
                formes[5] = new Forme("Heptagone","forme_polygone_heptagone", Forme.Difficulte.Difficile);
                formes[6] = new Forme("Octogone","forme_polygone_octogone", Forme.Difficulte.Difficile);
                formes[7] = new Forme("Ennéagone","forme_polygone_enneagone", Forme.Difficulte.Difficile);
                formes[8] = new Forme("Décagone","forme_polygone_decagone", Forme.Difficulte.Impossible);
                formes[9] = new Forme("Hendécagone","forme_polygone_hendecagone", Forme.Difficulte.Impossible);
                formes[10] = new Forme("Dodécagone","forme_polygone_dodecagone", Forme.Difficulte.Impossible);

                break;
            case ELLIPSES:
                formes = new Forme[1];
                formes[0] = new Forme("Cercle", "ic_regular_star_polygon_5_2", Forme.Difficulte.Facile);

                break;
            case SPIRALES:
                formes = new Forme[1];
                formes[0] = new Forme("À venir", "", null);

                break;
            case PARALLELOGRAMMES:
                formes = new Forme[7];
                formes[0] = new Forme("Carré", "forme_polygone_carre", Forme.Difficulte.Facile);
                formes[1] = new Forme("Rectangle 80%", "forme_parallelogramme_rectangle_810", Forme.Difficulte.Facile);
                formes[2] = new Forme("Rectangle 60%", "forme_parallelogramme_rectangle_610", Forme.Difficulte.Facile);
                formes[3] = new Forme("Rectangle 40%", "forme_parallelogramme_rectangle_410", Forme.Difficulte.Facile);
                formes[4] = new Forme("Rectangle 20%", "forme_parallelogramme_rectangle_210", Forme.Difficulte.Facile);
                formes[5] = new Forme("Parallélogramme 60%\n120 degrés", "forme_parallelogramme_120_610", Forme.Difficulte.Normale);
                formes[6] = new Forme("Parallélogramme 100%\n80 degrés", "forme_parallelogramme_80_1010", Forme.Difficulte.Normale);


                break;
            case EMOJI:
                formes = new Forme[3];
                formes[0] = new Forme("Content", "", Forme.Difficulte.Difficile);
                formes[1] = new Forme("Triste", "", Forme.Difficulte.Difficile);
                formes[2] = new Forme("Blazé", "", Forme.Difficulte.Normale);

                break;
            case ETOILES:
                formes = new Forme[4];
                formes[0] = new Forme("Étoile à 5 cotés {5/2}", "forme_polygone_etoile_5_2", Forme.Difficulte.Difficile);
                formes[1] = new Forme("Étoile à 7 cotés {7/2}", "forme_polygone_etoile_7_2", Forme.Difficulte.Impossible);
                formes[2] = new Forme("Étoile à 7 cotés {7/3}", "forme_polygone_etoile_7_3", Forme.Difficulte.Impossible);
                formes[3] = new Forme("Étoile à 8 cotés {8/3}", "forme_polygone_etoile_8_3", Forme.Difficulte.Impossible);

                break;
            case AUTRES:
                formes = new Forme[2];
                formes[0] = new Forme("Informatique", "forme_autre_informatique", Forme.Difficulte.Impossible);
                formes[1] = new Forme("Électrique", "forme_autre_electrique", Forme.Difficulte.Difficile);

                break;
            default:
                break;
        }

        adapteurFormes = new AdapteurArrayListeFormes(this, 0, formes);
        final ListView listeFormes = (ListView) findViewById(R.id.listeFormes);
        listeFormes.setAdapter(adapteurFormes);

        listeFormes.setOnItemClickListener(new AdapterView.OnItemClickListener()
        {
            public void onItemClick(AdapterView<?> parent, View view, int position, long id)
            {
                String messageBT = "#" + typeFormes + " " + position;
                telecommande.startActivity(formes[position].getImageDrawableString(), messageBT, getApplicationContext());
            }
        });

    }

    //Méthodes
    public void startActivity(int typeFormes, Context context)
    {
        Intent intent = new Intent(context, ListeFormes.class);
        intent.putExtra("typeFormes", typeFormes);
        context.startActivity(intent);
    }

    //custom ArrayAdapters
    class AdapteurArrayListeFormes extends ArrayAdapter<Forme>
    {

        private Context context;
        private Forme[] formes;

        public AdapteurArrayListeFormes(Context context, int resource, Forme[] objects)
        {
            super(context, resource, objects);

            this.context = context;
            this.formes = objects;
        }

        public View getView(int position, View convertView, ViewGroup parent)
        {
            String nomForme = formes[position].getImageDrawableString();

            //View d'un placement
            LayoutInflater inflater = (LayoutInflater) context.getSystemService(Activity.LAYOUT_INFLATER_SERVICE);
            View view = inflater.inflate(R.layout.layout_bouton_forme, null);
            //view.setPaddingRelative(20,20,20,20);

            ImageViewCarre imageForme = view.findViewById(R.id.imageForme);
            TextView nom = (TextView) view.findViewById(R.id.typeForme);
            TextView difficulte = (TextView) view.findViewById(R.id.difficulteForme);

            imageForme.setImageResource(getResources().getIdentifier(nomForme, "drawable", getContext().getPackageName()));
            if(!nomForme.equals("forme_autre_informatique") && !nomForme.equals("forme_autre_electrique"))
                imageForme.setColorFilter(ContextCompat.getColor(context, R.color.bleu));
            nom.setText(formes[position].getNom());
            difficulte.setText(formes[position].getDifficulte().name());
            switch(formes[position].getDifficulte())
            {
                case Facile:
                    difficulte.setTextColor(ContextCompat.getColor(context, R.color.vertUDeS));
                    break;
                case Normale:
                    difficulte.setTextColor(ContextCompat.getColor(context, R.color.jauneUDeS));
                    break;
                case Difficile:
                    difficulte.setTextColor(ContextCompat.getColor(context, R.color.rouge));
                    break;
                case Impossible:
                    difficulte.setTextColor(ContextCompat.getColor(context, R.color.mauve));
                    break;
                default:
                    break;
            }
            return view;
        }

        //Méthodes
        public Bitmap getBitmapGrandeur(Bitmap image, int maxSize)
        {
            int width, height;
            if(image != null)
            {
                width = image.getWidth();
                height = image.getHeight();

                float bitmapRatio = (float) width / (float) height;
                if (bitmapRatio > 1)
                {
                    width = maxSize;
                    height = (int) (width / bitmapRatio);
                } else
                {
                    height = maxSize;
                    width = (int) (height * bitmapRatio);
                }
                return Bitmap.createScaledBitmap(image, width, height, true);
            }
            else
            {
                return image;
            }
        }
    }
}


/*

*/